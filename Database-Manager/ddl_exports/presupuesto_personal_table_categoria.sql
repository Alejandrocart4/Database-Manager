-- Exportado por Database Manager
-- Fecha: 2026-02-22 20:54:59

CREATE TABLE `categoria` (
  `id_categoria` int(11) NOT NULL AUTO_INCREMENT,
  `nombre_categoria` varchar(100) DEFAULT NULL,
  `descripcion_categoria` varchar(255) DEFAULT NULL,
  `tipo_categoria` varchar(100) DEFAULT NULL,
  `nombre_iconoui_categoria` varchar(100) DEFAULT NULL,
  `colorhexa_categoria` varchar(10) DEFAULT NULL,
  `orden` int(11) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_categoria`)
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;
