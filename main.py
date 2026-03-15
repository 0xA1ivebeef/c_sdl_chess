
res = 0
with open(0) as file:
    for line in file:
        line = line.strip()
        res += int(line)

print(res)

