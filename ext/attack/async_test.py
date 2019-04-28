import asyncio
import time

async def say_after(delay, what):
    #asyncio.sleep(delay)
    time.sleep(delay)
    return what
    
async def main():
    # task1 = asyncio.create_task(say_after(1, 'hello'))

    # task2 = asyncio.create_task(
    #     say_after(2, 'world'))

    print(f"started at {time.strftime('%X')}")

    # Wait until both tasks are completed (should take
    # around 2 seconds.)
    print(await asyncio.create_task(say_after(1, 'hello')))
    print(await asyncio.create_task(say_after(2, 'world')))
    print("12")
    print(f"finished at {time.strftime('%X')}")

asyncio.run(main())
