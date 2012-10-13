@mode.com COM1: PARITY=E BAUD=333333
@copy /b top.bin COM1:
@echo "Update finished. Reconnect module to apply changes"
@pause