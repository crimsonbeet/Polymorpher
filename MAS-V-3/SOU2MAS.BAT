@echo Bat file for sou to mas-storage converting.
@echo 1 parameter required: file name without extension.
@echo It assumes mascot x-version and at least 1M extended memory.
@echo off
zmaker -l7000
m-conv -is%1 -om -c
