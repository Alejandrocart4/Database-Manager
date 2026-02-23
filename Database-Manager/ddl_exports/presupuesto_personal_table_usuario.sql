-- Exportado por Database Manager
-- Fecha: 2026-02-22 20:56:31

CREATE TABLE `usuario` (
  `id_usuario` int(11) NOT NULL AUTO_INCREMENT,
  `primer_nombre` varchar(100) DEFAULT NULL,
  `segundo_nombre` varchar(100) DEFAULT NULL,
  `primer_apellido` varchar(100) DEFAULT NULL,
  `segundo_apellido` varchar(100) DEFAULT NULL,
  `correo` varchar(150) DEFAULT NULL,
  `fecha_registro` datetime DEFAULT NULL,
  `salario_mensual_base` decimal(10,2) DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_usuario`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
