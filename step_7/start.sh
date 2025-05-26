#!/bin/bash

MODULE_NAME="userpace_logging"
PROC_FILE="/proc/app_monitor"
cleanup() {
    echo -e "\nвыход"
    if lsmod | grep -q "$MODULE_NAME"; then
        echo "выгрузка модуля $MODULE_NAME"
        sudo rmmod "$MODULE_NAME" >/dev/null
    fi
    if [ -f "$PROC_FILE" ];then
        sudo rm -f "$PROC_FILE"
    fi
    echo "Очистка завершена"
    exit 0
}

trap cleanup SIGINT #захват  ctrl+c

echo "компиляция и загрузка"
make clean >/dev/null
make >/dev/null 
sudo insmod "${MODULE_NAME}.ko"


echo -e "\nМониторинг userpace приложений"
echo "==================================="

# Бесконечный цикл вывода с задержкой
while true; do
    clear
    sudo cat "$PROC_FILE"
    echo "=============================="
    echo "обновлено $(date)"
    sleep 10
done
