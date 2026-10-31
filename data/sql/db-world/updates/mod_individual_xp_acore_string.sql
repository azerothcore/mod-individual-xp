SET @ENTRY:=100000;
DELETE FROM `acore_string` WHERE `entry` BETWEEN @ENTRY+0 AND @ENTRY+9;
INSERT INTO `acore_string` (`entry`, `content_default`) VALUES
(@ENTRY+0, 'This server is running the |cff4CFF00IndividualXpRate |rmodule.'),
(@ENTRY+1, '[XP] The Individual XP module is deactivated.'),
(@ENTRY+2, '[XP] Your Individual XP is currently disabled. Use .xp enable to re-enable it.'),
(@ENTRY+3, '|cffffffff[XP] Your current XP rate is {}.|r'),
(@ENTRY+4, '|cffffffff[XP] The maximum rate limit is {}.|r'),
(@ENTRY+5, '[XP] The minimum rate limit is 1.'),
(@ENTRY+6, '[XP] You have updated your XP rate to {}.'),
(@ENTRY+7, '[XP] You have disabled your XP gain.'),
(@ENTRY+8, '[XP] You have enabled your XP gain.'),
(@ENTRY+9, '[XP] You have restored your XP rate to the default value of {}.');

DELETE FROM `acore_string_locale` WHERE `entry` BETWEEN @ENTRY+0 AND @ENTRY+9 AND `locale` IN ('esES', 'esMX');
INSERT INTO `acore_string_locale` (`entry`, `locale`, `content`) VALUES
(@ENTRY+0, 'esES', 'Este servidor está ejecutando el módulo |cff4CFF00 mod-individual-xp.'),
(@ENTRY+0, 'esMX', 'Este servidor está ejecutando el módulo |cff4CFF00 mod-individual-xp.'),
(@ENTRY+1, 'esES', '[XP] El módulo XP individual está desactivado.'),
(@ENTRY+1, 'esMX', '[XP] El módulo XP individual está desactivado.'),
(@ENTRY+2, 'esES', '[XP] Su XP individual está actualmente desactivado. Utilice .xp enable para volver a activarlo.'),
(@ENTRY+2, 'esMX', '[XP] Su XP individual está actualmente desactivado. Utilice .xp enable para volver a activarlo.'),
(@ENTRY+3, 'esES', '|cffffffff[XP] Su experiencia actual es {}.|r'),
(@ENTRY+3, 'esMX', '|cffffffff[XP] su experiencia actual es {}.|r'),
(@ENTRY+4, 'esES', '|cffffffff[XP] El límite máximo de XP es {}.|r'),
(@ENTRY+4, 'esMX', '|cffffffff[XP] El límite máximo de XP es {}.|r'),
(@ENTRY+5, 'esES', '[XP] El límite mínimo de XP es 1.'),
(@ENTRY+5, 'esMX', 'El límite mínimo de XP es 1.'),
(@ENTRY+6, 'esES', '[XP] Has actualizado tu XP a {}.'),
(@ENTRY+6, 'esMX', '[XP] Has actualizado tu XP a {}.'),
(@ENTRY+7, 'esES', '[XP] Has desactivado tu ganancia de XP.'),
(@ENTRY+7, 'esMX', '[XP] Has desactivado tu ganancia de XP.'),
(@ENTRY+8, 'esES', '[XP] Has activado tu ganancia de XP.'),
(@ENTRY+8, 'esMX', '[XP] Has activado tu ganancia de XP.'),
(@ENTRY+9, 'esES', '[XP] Has restaurado tu tasa de XP al valor por defecto de {}.'),
(@ENTRY+9, 'esMX', '[XP] Has restaurado tu tasa de XP al valor por defecto de {}.');
