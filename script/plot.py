import matplotlib.pyplot as plt
import csv

# 数据
data = [12.12, 12.12, 19.65, 35.74, 37.51, 38.06, 45.24, 51.25, 53.11, 55.07, 69.30, 
        69.92, 71.35, 71.40, 71.38, 71.93, 77.49, 82.70, 82.64, 84.90, 90.18, 91.88, 
        96.06, 99.60, 99.60, 99.59, 100.00, 99.93, 99.92, 99.92, 99.91, 99.90, 
        99.84, 99.83, 99.83, 99.92, 99.92, 99.93, 99.93, 99.92, 99.91, 99.90, 
        99.90, 99.90, 99.90, 99.89, 99.81, 99.81, 99.80, 99.95, 99.77, 99.76]

# 假设每个数据点记录的时间间隔为10秒
time_data = [i*10 for i in range(len(data))]

# 保存数据到CSV文件
filename = 'exploration_percentage.csv'
with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['time', 'percentage'])
    for t, p in zip(time_data, data):
        writer.writerow([t, p])

# 读取并绘制图形
time_data = []
percentage_data = []
with open(filename, mode='r') as file:
    reader = csv.reader(file)
    next(reader)  # 跳过表头
    for row in reader:
        time_data.append(float(row[0]))
        percentage_data.append(float(row[1]))

# 绘制图形
plt.figure()
plt.plot(time_data, percentage_data, label='Exploration Efficiency')
plt.xlabel('Time (s)')
plt.ylabel('Exploration Percentage (%)')
plt.title('Exploration Efficiency Over Time')
plt.legend()
plt.grid(True)
plt.show()
