ALTER TABLE db_version CHANGE COLUMN required_12340_01_mangos_mangos_string required_12361_01_mangos_mangos_string bit;

DELETE FROM mangos_string WHERE entry IN (1509, 1510);
INSERT INTO mangos_string VALUES
(1509,'Pool %u not found (Maximal pool entry is %u)',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(1700,'Can\'t add item %u to vendor with unknown item type %u',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(1701,'Currency %u has maxCount = 0, but for currencies maxCount = buyCount, so it can\'t be 0 or less than that\'s currency precision (%u).',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
