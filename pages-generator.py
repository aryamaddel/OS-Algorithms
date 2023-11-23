import random
print(random.choices(range(0, 10), k=random.randint(10, 15),
      weights=[10 if i < 3 else 1 for i in range(10)]))
