SET @ENTRY := 100010;

DELETE FROM `acore_string` WHERE `entry` BETWEEN @ENTRY+0 AND @ENTRY+5;

INSERT INTO `acore_string` (`entry`, `content_default`, `locale_koKR`, `locale_frFR`, `locale_deDE`, `locale_zhCN`, `locale_zhTW`, `locale_esES`, `locale_esMX`, `locale_ruRU`) VALUES
(@ENTRY+0, '[XP] You don''t have permission to use XP commands.', '', '', '', '', '', '[XP] No tienes permiso para usar comandos de XP.', '[XP] No tienes permiso para usar comandos de XP.', ''),
(@ENTRY+1, '[XP] You have set {}''s XP rate to {}.', '', '', '', '', '', '[XP] Has establecido la tasa de XP de {} a {}.', '[XP] Has establecido la tasa de XP de {} a {}.', ''),
(@ENTRY+2, '[XP] {}''s current XP rate is {}.', '', '', '', '', '', '[XP] La tasa de XP actual de {} es {}.', '[XP] La tasa de XP actual de {} es {}.', ''),
(@ENTRY+3, '[XP] You have disabled XP gain for {}.', '', '', '', '', '', '[XP] Has desactivado la ganancia de XP para {}.', '[XP] Has desactivado la ganancia de XP para {}.', ''),
(@ENTRY+4, '[XP] You have enabled XP gain for {}.', '', '', '', '', '', '[XP] Has activado la ganancia de XP para {}.', '[XP] Has activado la ganancia de XP para {}.', ''),
(@ENTRY+5, '[XP] You have reset {}''s XP rate to the default value of {}.', '', '', '', '', '', '[XP] Has restablecido la tasa de XP de {} al valor predeterminado de {}.', '[XP] Has restablecido la tasa de XP de {} al valor predeterminado de {}.', '');
