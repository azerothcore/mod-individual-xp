SET @ENTRY := 100016;

DELETE FROM `acore_string` WHERE `entry` BETWEEN @ENTRY+0 AND @ENTRY+0;

INSERT INTO `acore_string` (`entry`, `content_default`, `locale_koKR`, `locale_frFR`, `locale_deDE`, `locale_zhCN`, `locale_zhTW`, `locale_esES`, `locale_esMX`, `locale_ruRU`) VALUES
(@ENTRY+0, '[XP] Character not found or is offline.', '', '', '', '', '', '[XP] Personagem no encontrado o está desconectado.', '[XP] Personagem no encontrado o está desconectado.', '');

UPDATE `command` SET `help` = 'Syntax: .xp set $rate [$charactername]\nSet your custom XP rate or another character''s XP rate.' WHERE `name` = 'xp set';
UPDATE `command` SET `help` = 'Syntax: .xp view [$charactername]\nView your current XP rate or another character''s XP rate.' WHERE `name` = 'xp view';
UPDATE `command` SET `help` = 'Syntax: .xp disable [$charactername]\nDisable the custom XP rate for yourself or another character.' WHERE `name` = 'xp disable';
UPDATE `command` SET `help` = 'Syntax: .xp enable [$charactername]\nEnable the custom XP rate for yourself or another character.' WHERE `name` = 'xp enable';
UPDATE `command` SET `help` = 'Syntax: .xp default [$charactername]\nSet your custom XP rate or another character''s XP rate to the default value.' WHERE `name` = 'xp default';
