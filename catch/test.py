import os
import sys
import time
import requests
import uuid
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.edge.service import Service
from selenium.webdriver.edge.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from urllib.parse import urljoin

def download_image(url, folder):
    """下载图片到指定文件夹"""
    try:
        print(f"正在下载图片: {url}")
        response = requests.get(url, stream=True, verify=False)  # 忽略 SSL 验证
        print(f"HTTP 状态码: {response.status_code}")
        if response.status_code == 200:
            # 提取图片文件名
            filename = url.split('/')[-1]
            filepath = os.path.join(folder, filename)

            # 检查文件是否已存在
            if os.path.exists(filepath):
                print(f"图片已存在，跳过: {filepath}")
                return

            # 保存图片
            with open(filepath, 'wb') as file:
                for chunk in response.iter_content(1024):
                    file.write(chunk)
            print(f"图片已保存: {filepath}")
        else:
            print(f"无法下载图片: {url}")
    except Exception as e:
        print(f"下载图片时出错: {e}")

def scrape_images(base_url, start_page, end_page, folder):
    """使用 Selenium 抓取指定页数范围的图片"""
    if not os.path.exists(folder):
        os.makedirs(folder)

    # 获取 msedgedriver.exe 的路径
    driver_path = os.path.join(os.getcwd(), "msedgedriver.exe")
    print(f"EdgeDriver 路径: {driver_path}")
    if not os.path.exists(driver_path):
        raise ValueError(f"The path is not a valid file: {driver_path}")

    service = Service(driver_path)
    options = Options()
    options.add_argument('--headless')  # 无头模式
    options.add_argument('--disable-gpu')
    options.add_argument('--no-sandbox')
    options.add_argument('user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36')
    driver = webdriver.Edge(service=service, options=options)

    for page in range(start_page, end_page + 1):
        print(f"正在抓取第 {page} 页...")
        url = f"{base_url}?page={page}"
        try:
            driver.get(url)
            time.sleep(2)  # 等待页面加载

            # 模拟滚动页面，加载懒加载图片
            for _ in range(3):  # 滚动 3 次
                driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
                time.sleep(2)  # 等待图片加载

            # 显式等待图片加载完成
            WebDriverWait(driver, 10).until(
                EC.presence_of_all_elements_located((By.TAG_NAME, 'img'))
            )

            # 查找所有图片标签
            images = driver.find_elements(By.TAG_NAME, 'img')
            for img in images:
                # 检查多个属性以获取图片 URL
                img_url = img.get_attribute('data-src') or img.get_attribute('src')
                if img_url:
                    # 如果是相对路径，拼接完整 URL
                    img_url = urljoin(base_url, img_url)
                    print(f"找到图片 URL: {img_url}")
                    download_image(img_url, folder)
        except Exception as e:
            print(f"抓取页面时出错: {e}")

    driver.quit()

if __name__ == "__main__":
    # 网站的基础 URL
    base_url = "http://www.photo.uestc.edu.cn/home-list/new"
    # 用户输入爬取的起始页和结束页
    start_page = int(input("请输入要爬取的起始页: "))
    end_page = int(input("请输入要爬取的结束页: "))
    # 保存图片的文件夹
    save_folder = "images"

    scrape_images(base_url, start_page, end_page, save_folder)