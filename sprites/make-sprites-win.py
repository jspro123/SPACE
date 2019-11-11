#!/bin/python3

#This exists mainly to work around the (many) problems with doing (simple) CLI things on windows. Like lack of shell globbing.

name = "the-planet"
name2 = "screen"
name3 = "trade-font"
gimp = "C:\\Program Files\\GIMP 2\\bin\\gimp-console-2.10.exe"

import subprocess
import os
import shutil

if os.path.exists("../dist/" + name + ".png"):
	os.unlink("../dist/" + name + ".png")

if os.path.exists("../dist/" + name + ".atlas"):
	os.unlink("../dist/" + name + ".atlas")

if os.path.exists(name):
	shutil.rmtree(name)

subprocess.run([
	"python",
	"extract-sprites.py",
	name + ".list",
	name,
	"--gimp", gimp
], check=True)

subprocess.run([
	"python",
	"extract-sprites.py",
	name2 + ".list",
	name,
	"--gimp", gimp
], check=True)

subprocess.run([
	"python",
	"extract-sprites.py",
	name3 + ".list",
	name,
	"--gimp", gimp
], check=True)


pngs = []
for root, dirs, files in os.walk(name):
	for f in files:
		if f.endswith(".png"):
			pngs.append(root + "/" + f)
	break

print(pngs)

subprocess.run([
	"pack-sprites.exe",
	"../dist/" + name
] + pngs, check=True)
