#!venv/bin/python

import logging
import sys
import subprocess
from os import getenv

from aiogram import Bot, Dispatcher, executor, types

bot = Bot(token=getenv("BOT_TOKEN"))
dp = Dispatcher(bot)
logging.basicConfig(stream=sys.stderr, level=logging.INFO)


def run_matlang_script(script: str):
    return subprocess.run(["wsl", "--exec", "./matlang"],
                          input=script.encode("utf-8"),
                          stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE,
                          shell=True)


@dp.message_handler()
async def code(message: types.Message):
    output = run_matlang_script(message.text)
    if output.returncode != 0:
        await message.reply("`" + output.stderr.decode("utf-8") + "`",
                            parse_mode="Markdown")
    else:
        await message.reply("`" + output.stdout.decode("utf-8") + "`",
                            parse_mode="Markdown")


if __name__ == "__main__":
    built = subprocess.run(["wsl", "--exec", "./build.sh"], shell=True)
    if built.returncode != 0:
        raise RuntimeError(built.args)
    executor.start_polling(dp, skip_updates=True)
