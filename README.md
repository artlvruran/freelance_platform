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
