# Generic_Netlink_Communication
Simple implementation of client-server applications from user space to user space.
# Задача
Нужно написать два userspace-приложения (клиент и сервер)б которые общаются по протоколу Generic Netlink.
Сервер ожидает от клиента запрос в формате JSON подобного вида:  \
{ "action": "plus", "argument_1": 1, "argument_2": 2 }  \
И возвращает результат подобного вида: \
{ "result": 3 }
# Порядок решения и вывод программ
1. Клонируем репозиторий  \
`$ gitclone https://github.com/ProProperDev/Generic_Netlink_Communication`  \
2. Собираем проект  \
`$ make`
3. Сначала запускаем приложение сервера, затем в соседнем терминале приложение клиента и перенаправляем в него поток из файла с тестовыми запросами \
`$ sudo ./server`
`$ sudo ./client < Test_JSON_requests`  \
# Вывод программ
[Screenshot](https://github.com/ProProperDev/Generic_Netlink_Communication/blob/main/Screenshots/workscreen.png)  \
Пояснение: при прогонке приложения сервера Valgrind пишет, что 532 байта в 1 блоке были обнаружены как "still reachable". Это означает, что на момент завершения программы на эту память все еще существуют указатели, и Valgrind считает эту память "доступной" (reachable). Данное сообщение связано с тем, что сервер, ожидая сообщения, выделяет память в бесконечном цикле в функции `receive_gnl_message(...)` через `malloc()` для записи туда полученного сообщения и освобождает в конце цикла. То есть эта область памяти при принудительном завершении числится как "still reachable" и освободится при завершении всей программы.  \
[Valgrind_Test](https://github.com/ProProperDev/Generic_Netlink_Communication/blob/main/Screenshots/valgrind_expl.png)
