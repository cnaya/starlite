DROP DATABASE  IF EXISTS `webcppd`;
CREATE DATABASE `webcppd` ;
alter database `webcppd` character set utf8;
alter database `webcppd` collate utf8_general_ci;
USE `webcppd`;


CREATE TABLE `article` (
    `id` INT(11) NOT NULL AUTO_INCREMENT,
    `user` INT(11) NOT NULL,
    `head` VARCHAR(50) NOT NULL,
    `body` TEXT NOT NULL,
    `abstract` VARCHAR(200) NOT NULL,
    `keywords` VARCHAR(80) NOT NULL,
    `publish` BOOL NOT NULL DEFAULT TRUE,
    `ratable` BOOL NOT NULL DEFAULT TRUE,
    `visitor` INT(11) NOT NULL DEFAULT 0,
    `message` INT(11) NOT NULL DEFAULT 0,
    `permission` BOOL NOT NULL DEFAULT TRUE,
    `created` DATETIME NOT NULL,
    PRIMARY KEY (`id`),
    KEY (`user`),
    FULLTEXT KEY ( `head` , `keywords` , `abstract` , `body` ),
    KEY (`publish`),
    KEY (`visitor`),
    KEY (`permission`),
    KEY (`created`)
)  ENGINE=Mroonga;
alter table `article` character set  utf8;
alter table `article` collate utf8_general_ci;

CREATE TABLE `article_user` (
    `id` INT(11) NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(50) NOT NULL DEFAULT 'Guess',
    `password` VARCHAR(100) NOT NULL,
    `email` VARCHAR(50) NOT NULL,
    `about` varchar(200) NOT NULL DEFAULT '我是一个程序员。',
    `image` VARCHAR(100) NOT NULL DEFAULT '/assets/64x64.png',
    `permission` BOOL NOT NULL DEFAULT TRUE,
    `created` DATETIME NOT NULL,
    PRIMARY KEY (`id`),
    KEY (`password`),
    UNIQUE KEY (`name` , `email`),
    KEY (`permission`),
    KEY (`created`)
)  ENGINE=INNODB;
alter table `article_user` character set  utf8;
alter table `article_user` collate utf8_general_ci;

insert into `article_user`(`email`,`password`,`created`)values('admin@webcpp.net',md5('123456'),now());
/*
CREATE TABLE  `article_message` (
    `id` INT(11) NOT NULL AUTO_INCREMENT,
    `user` INT(11) NOT NULL,
    `article` INT(11) NOT NULL,
    `content` VARCHAR(100) NOT NULL,
    `reply` INT(11) NOT NULL DEFAULT 0,
    `permission` BOOL NOT NULL DEFAULT TRUE,
    `created` DATETIME NOT NULL,
    PRIMARY KEY (`id`),
    KEY (`user`),
    KEY (`article`),
    FULLTEXT KEY ( `content` ),
    KEY (`reply`),
    KEY (`permission`),
    KEY (`created`)
)  ENGINE=Mroonga;
alter table `article_message` character set utf8;
alter table `article_message` collate utf8_general_ci;
*/

CREATE VIEW `get_lastest_articles` AS
    SELECT 
        A.`id` AS `pid`,
        A.`head`,
        A.`abstract`,
        A.`message`,
        A.`visitor`,
        A.`created`,
        B.`id` AS `uid`,
        B.`name` AS `uname`,
        B.`image` AS `uimage`
    FROM
        `article` A
            INNER JOIN
        `article_user` B ON B.`id` = A.`user`
    WHERE
        A.`permission` = TRUE
            AND A.`publish` = TRUE
    ORDER BY A.`created` DESC
;
