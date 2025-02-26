import requests

url1 = 'https://scd.dfilip.xyz/lab1/task1'
url1_check = 'https://scd.dfilip.xyz/lab1/task1/check'

data = {
    'nume': 'Ionescu Cristina',
    'grupa': '341C3',
    'userId': 1
}

post_data = {
    "secret": "SCDisNice"
}

headers = {
    "secret2": "SCDisBest"
}

print("-----------Task1----------\n")

response_task1 = requests.post(url1, params=data, data=post_data, headers=headers)
print(response_task1.text)

print("\n")

response_check = requests.post(url1_check, params=data, data=post_data, headers=headers)
print(response_check.text)

print("\n-----------Task2----------\n")

url2 = 'https://scd.dfilip.xyz/lab1/task2'

payload = {
    'username': 'scd',
    'password': 'admin',
    'nume': 'Ionescu Cristina'
}

response_task2 = requests.post(url2, json=payload)
print(response_task2.text)

print("\n")

print("\n-----------Task3----------\n")

url3 = "https://scd.dfilip.xyz/lab1/task3/login"
url3_check = 'https://scd.dfilip.xyz/lab1/task3/check'

response_task3 = requests.post(url3, json=payload)
print(response_task3.text)

print("\n")

response_check = requests.get(url3_check, cookies=response_task3.cookies)
print(response_check.text)

