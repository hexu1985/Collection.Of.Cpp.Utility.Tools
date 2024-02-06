import time


def plus_one(x):
    time.sleep(0.5)
    return x + 1


def mul_two(x):
    time.sleep(0.5)
    return x * 2


if __name__ == '__main__':
    for i in range(5):
        start_time = time.time()
        output = mul_two(plus_one(i))
        print(f"{output}: {time.time() - start_time:.1f}s")
