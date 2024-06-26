# Основные классы (модели взаимодействия с БД)
* #### User
  * #### Contractor - класс нанимателя
  * #### Employee - класс работника
* #### Project - класс проекта(заказа)
  * #### LongTermJob - долгосрочный проект
  * #### Task - обычный заказ
  * #### Contest - конкурсный заказ
* #### State - состояние заказа
  * #### NotStarted - не начат
  * #### Preparing - процесс приёма заявок
  * #### Processing - основная стадия выполнения заказа
  * #### Completed - заказ завершен
* #### Bid - класс заявки
* #### BidState - статус заявки
    * #### BidStateConsidering - на рассмотрении
    * #### BidStateApproved - принята
    * #### BidStateRejected - отклонена
![diagrams/frealnceplatform Class diagram.png](https://github.com/artlvruran/freelance_platform/blob/checkpoint_1/diagrams/frealnceplatform%20Class%20diagram.png)
# Паттерны 
  * Observer (функционал подписок)
  * State (состояния заказов и состояния заявок)
  * Factory (ProjectFactory)
# Сценарии использования
- Для взаимодействия с системой пользователь должен пройти регистрацию, и выбрать нужную роль: заказчик или исполнитель.
- Любой пользователь может подписаться на (отписаться от) заказчика.
- Исполнитель может подавать заявки на проекты.
- Заказчик может создавать новые проекты, менять их этап разработки, одобрять, отклонять и отменять(менять состояние заявки с accepted на rejected) заявки исполнителей.
- При смене этапа разработки над проектом всем подписчикам его заказчика направляется уведомление об этом.
# Запуск
##### Введите код взаимодействия с классами в main.cpp
##### Запустите:
``` bash
cmake -S . -B build
cmake --build build  --target FreelancePlatform
cd build && ./FreelancePlatform
```

###### Требования и библиотеки:
* ###### CMake - 3.27
* ###### С++ - 17
* ###### sqlite3
* ###### soci_core
* ###### soci_sqlite3

### How to run on local host:
1. ###### run following command:

# How to run on local host:
1. ###### run folowing command:
```bash
./run.sh
```
2. ###### go to http://localhost:4000/