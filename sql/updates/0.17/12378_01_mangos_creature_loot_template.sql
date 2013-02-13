ALTER TABLE db_version CHANGE COLUMN required_12377_01_mangos_dbscripts required_12378_01_mangos_creature_loot_template bit;


ALTER TABLE creature_loot_template DROP COLUMN lootcondition, DROP COLUMN condition_value1, DROP COLUMN condition_value2;
