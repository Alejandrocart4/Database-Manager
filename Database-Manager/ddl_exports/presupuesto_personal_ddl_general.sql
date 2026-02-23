-- DDL General (export)
-- Generado: 2026-02-22 20:56:02

CREATE DATABASE IF NOT EXISTS `presupuesto_personal`;
USE `presupuesto_personal`;

-- Tabla: categoria
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

-- Tabla: meta_ahorro
CREATE TABLE `meta_ahorro` (
  `id_meta_ahorro` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario` int(11) DEFAULT NULL,
  `id_subcategoria` int(11) DEFAULT NULL,
  `nombre` varchar(150) DEFAULT NULL,
  `descripcion` varchar(255) DEFAULT NULL,
  `monto_total` decimal(10,2) DEFAULT NULL,
  `monto_ahorrado` decimal(10,2) DEFAULT NULL,
  `fecha_inicio` date DEFAULT NULL,
  `fecha_objetivo` date DEFAULT NULL,
  `prioridad` int(11) DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_meta_ahorro`),
  KEY `fk_meta_usuario` (`id_usuario`),
  KEY `fk_meta_subcategoria` (`id_subcategoria`),
  CONSTRAINT `fk_meta_subcategoria` FOREIGN KEY (`id_subcategoria`) REFERENCES `subcategoria` (`id_subcategoria`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_meta_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuario` (`id_usuario`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: obligacion_fija
CREATE TABLE `obligacion_fija` (
  `id_obligacion_fija` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario` int(11) DEFAULT NULL,
  `id_subcategoria` int(11) DEFAULT NULL,
  `nombre_obligacion` varchar(150) DEFAULT NULL,
  `descripcion` varchar(255) DEFAULT NULL,
  `monto_fijo_mensual` decimal(10,2) DEFAULT NULL,
  `dia_vencimiento` int(11) DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL,
  `fecha_inicio` date DEFAULT NULL,
  `fecha_finalizacion` date DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_obligacion_fija`),
  UNIQUE KEY `id_subcategoria` (`id_subcategoria`),
  KEY `fk_obligacion_usuario` (`id_usuario`),
  CONSTRAINT `fk_obligacion_subcategoria` FOREIGN KEY (`id_subcategoria`) REFERENCES `subcategoria` (`id_subcategoria`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_obligacion_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuario` (`id_usuario`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: presupuesto
CREATE TABLE `presupuesto` (
  `id_presupuesto` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario` int(11) DEFAULT NULL,
  `nombre_presupuesto` varchar(150) DEFAULT NULL,
  `anio_inicio` int(11) DEFAULT NULL,
  `mes_inicio` int(11) DEFAULT NULL,
  `anio_fin` int(11) DEFAULT NULL,
  `mes_fin` int(11) DEFAULT NULL,
  `total_ingresos` decimal(10,2) DEFAULT NULL,
  `total_gastos` decimal(10,2) DEFAULT NULL,
  `total_ahorro` decimal(10,2) DEFAULT NULL,
  `fecha_hora_creacion` datetime DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_presupuesto`),
  KEY `fk_presupuesto_usuario` (`id_usuario`),
  CONSTRAINT `fk_presupuesto_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuario` (`id_usuario`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: presupuesto_detalle
CREATE TABLE `presupuesto_detalle` (
  `id_presupuesto_detalle` int(11) NOT NULL AUTO_INCREMENT,
  `id_presupuesto` int(11) DEFAULT NULL,
  `id_subcategoria` int(11) DEFAULT NULL,
  `monto_mensual` decimal(10,2) DEFAULT NULL,
  `observacion` varchar(255) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_presupuesto_detalle`),
  KEY `fk_detalle_presupuesto` (`id_presupuesto`),
  KEY `fk_detalle_subcategoria` (`id_subcategoria`),
  CONSTRAINT `fk_detalle_presupuesto` FOREIGN KEY (`id_presupuesto`) REFERENCES `presupuesto` (`id_presupuesto`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_detalle_subcategoria` FOREIGN KEY (`id_subcategoria`) REFERENCES `subcategoria` (`id_subcategoria`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: subcategoria
CREATE TABLE `subcategoria` (
  `id_subcategoria` int(11) NOT NULL AUTO_INCREMENT,
  `id_categoria` int(11) DEFAULT NULL,
  `nombre_subcategoria` varchar(100) DEFAULT NULL,
  `estado` varchar(50) DEFAULT NULL,
  `auto` tinyint(1) DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_subcategoria`),
  KEY `fk_subcategoria_categoria` (`id_categoria`),
  CONSTRAINT `fk_subcategoria_categoria` FOREIGN KEY (`id_categoria`) REFERENCES `categoria` (`id_categoria`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: transaccion
CREATE TABLE `transaccion` (
  `id_transaccion` int(11) NOT NULL AUTO_INCREMENT,
  `id_usuario` int(11) DEFAULT NULL,
  `id_presupuesto` int(11) DEFAULT NULL,
  `id_subcategoria` int(11) DEFAULT NULL,
  `id_obligacion_fija` int(11) DEFAULT NULL,
  `tipo_transaccion` varchar(50) DEFAULT NULL,
  `descripcion` varchar(255) DEFAULT NULL,
  `monto` decimal(10,2) DEFAULT NULL,
  `fecha` date DEFAULT NULL,
  `metodo_pago` varchar(100) DEFAULT NULL,
  `numero_factura` varchar(100) DEFAULT NULL,
  `observaciones` varchar(255) DEFAULT NULL,
  `fecha_hora_registro` datetime DEFAULT NULL,
  `creado_por` varchar(100) DEFAULT NULL,
  `creado_en` datetime DEFAULT NULL,
  `modificado_por` varchar(100) DEFAULT NULL,
  `modificado_en` datetime DEFAULT NULL,
  PRIMARY KEY (`id_transaccion`),
  KEY `fk_transaccion_usuario` (`id_usuario`),
  KEY `fk_transaccion_presupuesto` (`id_presupuesto`),
  KEY `fk_transaccion_subcategoria` (`id_subcategoria`),
  KEY `fk_transaccion_obligacion` (`id_obligacion_fija`),
  CONSTRAINT `fk_transaccion_obligacion` FOREIGN KEY (`id_obligacion_fija`) REFERENCES `obligacion_fija` (`id_obligacion_fija`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_transaccion_presupuesto` FOREIGN KEY (`id_presupuesto`) REFERENCES `presupuesto` (`id_presupuesto`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_transaccion_subcategoria` FOREIGN KEY (`id_subcategoria`) REFERENCES `subcategoria` (`id_subcategoria`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_transaccion_usuario` FOREIGN KEY (`id_usuario`) REFERENCES `usuario` (`id_usuario`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_uca1400_ai_ci;

-- Tabla: usuario
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

-- Función: fn_dias_restantes_meta
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_dias_restantes_meta`(p_fecha_objetivo DATE
) RETURNS int(11)
    DETERMINISTIC
BEGIN
    RETURN DATEDIFF(p_fecha_objetivo, CURDATE());
END$$
DELIMITER ;

-- Función: fn_es_mes_vigente
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_es_mes_vigente`(p_anio INT,
    p_mes INT
) RETURNS tinyint(4)
    DETERMINISTIC
BEGIN
    IF p_anio = YEAR(CURDATE()) AND p_mes = MONTH(CURDATE()) THEN
        RETURN 1;
    END IF;
    RETURN 0;
END$$
DELIMITER ;

-- Función: fn_mes_nombre
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_mes_nombre`(p_mes INT
) RETURNS varchar(15) CHARSET utf8mb4 COLLATE utf8mb4_general_ci
    DETERMINISTIC
BEGIN
    RETURN ELT(p_mes, 'Enero','Febrero','Marzo','Abril','Mayo','Junio',
               'Julio','Agosto','Septiembre','Octubre','Noviembre','Diciembre');
END$$
DELIMITER ;

-- Función: fn_monto_ejecutado_subcategoria
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_monto_ejecutado_subcategoria`(p_id_subcategoria INT,
    p_anio INT,
    p_mes INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_ejecutado DECIMAL(10,2);

    SELECT SUM(monto) INTO v_ejecutado
    FROM transaccion
    WHERE id_subcategoria = p_id_subcategoria
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;

    RETURN IFNULL(v_ejecutado, 0);
END$$
DELIMITER ;

-- Función: fn_obtener_categoria_por_sub
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_obtener_categoria_por_sub`(p_id_subcategoria INT
) RETURNS int(11)
    DETERMINISTIC
BEGIN
    DECLARE v_categoria INT;

    SELECT id_categoria INTO v_categoria
    FROM subcategoria
    WHERE id_subcategoria = p_id_subcategoria;

    RETURN v_categoria;
END$$
DELIMITER ;

-- Función: fn_porcentaje_ejecucion
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_porcentaje_ejecucion`(p_ejecutado DECIMAL(10,2),
    p_presupuesto DECIMAL(10,2)
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    IF p_presupuesto = 0 THEN
        RETURN 0;
    END IF;

    RETURN (p_ejecutado / p_presupuesto) * 100;
END$$
DELIMITER ;

-- Función: fn_presupuesto_mensual
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_presupuesto_mensual`(p_id_presupuesto INT,
    p_id_subcategoria INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_monto DECIMAL(10,2);

    SELECT monto_mensual INTO v_monto
    FROM presupuesto_detalle
    WHERE id_presupuesto = p_id_presupuesto
      AND id_subcategoria = p_id_subcategoria;

    RETURN IFNULL(v_monto, 0);
END$$
DELIMITER ;

-- Función: fn_promedio_gasto_subcategoria
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_promedio_gasto_subcategoria`(p_id_subcategoria INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_promedio DECIMAL(10,2);

    SELECT AVG(monto) INTO v_promedio
    FROM transaccion
    WHERE id_subcategoria = p_id_subcategoria
      AND tipo_transaccion = 'gasto';

    RETURN IFNULL(v_promedio, 0);
END$$
DELIMITER ;

-- Función: fn_suma_presupuesto_categoria
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_suma_presupuesto_categoria`(p_id_categoria INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_total DECIMAL(10,2);

    SELECT SUM(pd.monto_mensual) INTO v_total
    FROM presupuesto_detalle pd
    JOIN subcategoria sc ON sc.id_subcategoria = pd.id_subcategoria
    WHERE sc.id_categoria = p_id_categoria;

    RETURN IFNULL(v_total, 0);
END$$
DELIMITER ;

-- Función: fn_total_ahorro_usuario
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_total_ahorro_usuario`(p_id_usuario INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_total DECIMAL(10,2);

    SELECT SUM(monto_ahorrado) INTO v_total
    FROM meta_ahorro
    WHERE id_usuario = p_id_usuario;

    RETURN IFNULL(v_total, 0);
END$$
DELIMITER ;

-- Función: fn_total_gastos_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_total_gastos_mes`(p_id_usuario INT,
    p_anio INT,
    p_mes INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_total DECIMAL(10,2);

    SELECT SUM(monto) INTO v_total
    FROM transaccion
    WHERE id_usuario = p_id_usuario
      AND tipo_transaccion = 'gasto'
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;

    RETURN IFNULL(v_total, 0);
END$$
DELIMITER ;

-- Función: fn_total_ingresos_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `fn_total_ingresos_mes`(p_id_usuario INT,
    p_anio INT,
    p_mes INT
) RETURNS decimal(10,2)
    DETERMINISTIC
BEGIN
    DECLARE v_total DECIMAL(10,2);

    SELECT SUM(monto) INTO v_total
    FROM transaccion
    WHERE id_usuario = p_id_usuario
      AND tipo_transaccion = 'ingreso'
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;

    RETURN IFNULL(v_total, 0);
END$$
DELIMITER ;

-- Procedimiento: sp_actualizar_todas_metas_ahorro
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_actualizar_todas_metas_ahorro`(
    IN p_id_usuario INT
)
BEGIN
    UPDATE meta_ahorro m
    JOIN (
        SELECT id_subcategoria, SUM(monto) AS total
        FROM transaccion
        WHERE tipo_transaccion = 'ahorro'
        GROUP BY id_subcategoria
    ) t ON m.id_subcategoria = t.id_subcategoria
    SET m.monto_ahorrado = t.total
    WHERE m.id_usuario = p_id_usuario;
END$$
DELIMITER ;

-- Procedimiento: sp_calcular_balance_mensual
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_calcular_balance_mensual`(
    IN p_id_usuario INT,
    IN p_id_presupuesto INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    SELECT
        SUM(IF(tipo_transaccion='ingreso', monto, 0)) AS total_ingresos,
        SUM(IF(tipo_transaccion='gasto', monto, 0)) AS total_gastos,
        SUM(IF(tipo_transaccion='ahorro', monto, 0)) AS total_ahorros,
        SUM(IF(tipo_transaccion='ingreso', monto, 0))
        - SUM(IF(tipo_transaccion='gasto', monto, 0))
        - SUM(IF(tipo_transaccion='ahorro', monto, 0)) AS balance_final
    FROM transaccion
    WHERE id_usuario = p_id_usuario
      AND id_presupuesto = p_id_presupuesto
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;
END$$
DELIMITER ;

-- Procedimiento: sp_calcular_monto_ejecutado_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_calcular_monto_ejecutado_mes`(
    IN p_id_subcategoria INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    SELECT SUM(monto) AS monto_ejecutado
    FROM transaccion
    WHERE id_subcategoria = p_id_subcategoria
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;
END$$
DELIMITER ;

-- Procedimiento: sp_calcular_porcentaje_ejecucion_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_calcular_porcentaje_ejecucion_mes`(
    IN p_id_subcategoria INT,
    IN p_id_presupuesto INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    DECLARE v_pres DECIMAL(10,2) DEFAULT 0;
    DECLARE v_exec DECIMAL(10,2) DEFAULT 0;

    SELECT monto_mensual INTO v_pres
    FROM presupuesto_detalle
    WHERE id_subcategoria = p_id_subcategoria
      AND id_presupuesto = p_id_presupuesto;

    SELECT SUM(monto) INTO v_exec
    FROM transaccion
    WHERE id_subcategoria = p_id_subcategoria
      AND YEAR(fecha) = p_anio
      AND MONTH(fecha) = p_mes;

    SELECT (v_exec / v_pres) * 100 AS porcentaje;
END$$
DELIMITER ;

-- Procedimiento: sp_categoria_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_categoria_create`(
    IN p_nombre_categoria        VARCHAR(100),
    IN p_descripcion_categoria   VARCHAR(255),
    IN p_tipo_categoria          VARCHAR(100),
    IN p_nombre_icono            VARCHAR(100),
    IN p_color_hex               VARCHAR(10),
    IN p_orden                   INT,
    IN p_creado_por              VARCHAR(100)
)
BEGIN
    INSERT INTO categoria(
        nombre_categoria, descripcion_categoria, tipo_categoria,
        nombre_iconoui_categoria, colorhexa_categoria, orden,
        creado_por, creado_en
    )
    VALUES(
        p_nombre_categoria, p_descripcion_categoria, p_tipo_categoria,
        p_nombre_icono, p_color_hex, p_orden,
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_categoria_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_categoria_delete`(
    IN p_id_categoria INT
)
BEGIN
    DELETE FROM categoria
    WHERE id_categoria = p_id_categoria;
END$$
DELIMITER ;

-- Procedimiento: sp_categoria_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_categoria_get`(
    IN p_id_categoria INT
)
BEGIN
    SELECT *
    FROM categoria
    WHERE id_categoria = p_id_categoria;
END$$
DELIMITER ;

-- Procedimiento: sp_categoria_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_categoria_list`()
BEGIN
    SELECT *
    FROM categoria;
END$$
DELIMITER ;

-- Procedimiento: sp_categoria_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_categoria_update`(
    IN p_id_categoria            INT,
    IN p_nombre_categoria        VARCHAR(100),
    IN p_descripcion_categoria   VARCHAR(255),
    IN p_tipo_categoria          VARCHAR(100),
    IN p_nombre_icono            VARCHAR(100),
    IN p_color_hex               VARCHAR(10),
    IN p_orden                   INT,
    IN p_modificado_por          VARCHAR(100)
)
BEGIN
    UPDATE categoria
    SET
        nombre_categoria = p_nombre_categoria,
        descripcion_categoria = p_descripcion_categoria,
        tipo_categoria = p_tipo_categoria,
        nombre_iconoui_categoria = p_nombre_icono,
        colorhexa_categoria = p_color_hex,
        orden = p_orden,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_categoria = p_id_categoria;
END$$
DELIMITER ;

-- Procedimiento: sp_cerrar_presupuesto
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_cerrar_presupuesto`(
    IN p_id_presupuesto INT,
    IN p_modificado_por VARCHAR(100)
)
BEGIN
    UPDATE presupuesto
    SET estado = 'cerrado',
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_presupuesto = p_id_presupuesto;
END$$
DELIMITER ;

-- Procedimiento: sp_crear_presupuesto_completo
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_crear_presupuesto_completo`(
    IN p_id_usuario INT,
    IN p_nombre_presupuesto VARCHAR(150),
    IN p_anio_inicio INT,
    IN p_mes_inicio INT,
    IN p_anio_fin INT,
    IN p_mes_fin INT,
    IN p_estado VARCHAR(50),
    IN p_lista_detalles_json JSON,
    IN p_creado_por VARCHAR(100)
)
BEGIN
    DECLARE v_id_presupuesto INT;

    INSERT INTO presupuesto(
        id_usuario, nombre_presupuesto,
        anio_inicio, mes_inicio,
        anio_fin, mes_fin,
        estado, fecha_hora_creacion,
        creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_nombre_presupuesto,
        p_anio_inicio, p_mes_inicio,
        p_anio_fin, p_mes_fin,
        p_estado, NOW(),
        p_creado_por, NOW()
    );

    SET v_id_presupuesto = LAST_INSERT_ID();

    INSERT INTO presupuesto_detalle(id_presupuesto, id_subcategoria, monto_mensual, observacion, creado_por, creado_en)
    SELECT 
        v_id_presupuesto,
        JSON_EXTRACT(j.value, '$.id_subcategoria'),
        JSON_EXTRACT(j.value, '$.monto_mensual'),
        JSON_EXTRACT(j.value, '$.observacion'),
        p_creado_por,
        NOW()
    FROM JSON_TABLE(p_lista_detalles_json, "$[*]" COLUMNS(value JSON PATH "$")) AS j;
END$$
DELIMITER ;

-- Procedimiento: sp_distribuir_presupuesto_automatico
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_distribuir_presupuesto_automatico`(
    IN p_id_usuario INT,
    IN p_id_presupuesto INT
)
BEGIN
    SELECT sc.id_subcategoria, AVG(t.monto) AS promedio
    FROM transaccion t
    JOIN subcategoria sc ON sc.id_subcategoria = t.id_subcategoria
    WHERE t.id_usuario = p_id_usuario
    GROUP BY sc.id_subcategoria;
END$$
DELIMITER ;

-- Procedimiento: sp_meta_ahorro_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_meta_ahorro_create`(
    IN p_id_usuario        INT,
    IN p_id_subcategoria   INT,
    IN p_nombre            VARCHAR(150),
    IN p_descripcion       VARCHAR(255),
    IN p_monto_total       DECIMAL(10,2),
    IN p_monto_ahorrado    DECIMAL(10,2),
    IN p_fecha_inicio      DATE,
    IN p_fecha_objetivo    DATE,
    IN p_prioridad         INT,
    IN p_estado            VARCHAR(50),
    IN p_creado_por        VARCHAR(100)
)
BEGIN
    INSERT INTO meta_ahorro(
        id_usuario, id_subcategoria, nombre, descripcion,
        monto_total, monto_ahorrado, fecha_inicio, fecha_objetivo,
        prioridad, estado, creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_id_subcategoria, p_nombre, p_descripcion,
        p_monto_total, p_monto_ahorrado, p_fecha_inicio, p_fecha_objetivo,
        p_prioridad, p_estado, p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_meta_ahorro_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_meta_ahorro_delete`(
    IN p_id_meta_ahorro INT
)
BEGIN
    DELETE FROM meta_ahorro
    WHERE id_meta_ahorro = p_id_meta_ahorro;
END$$
DELIMITER ;

-- Procedimiento: sp_meta_ahorro_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_meta_ahorro_get`(
    IN p_id_meta_ahorro INT
)
BEGIN
    SELECT *
    FROM meta_ahorro
    WHERE id_meta_ahorro = p_id_meta_ahorro;
END$$
DELIMITER ;

-- Procedimiento: sp_meta_ahorro_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_meta_ahorro_list`()
BEGIN
    SELECT *
    FROM meta_ahorro;
END$$
DELIMITER ;

-- Procedimiento: sp_meta_ahorro_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_meta_ahorro_update`(
    IN p_id_meta_ahorro    INT,
    IN p_nombre            VARCHAR(150),
    IN p_descripcion       VARCHAR(255),
    IN p_monto_total       DECIMAL(10,2),
    IN p_monto_ahorrado    DECIMAL(10,2),
    IN p_fecha_objetivo    DATE,
    IN p_prioridad         INT,
    IN p_estado            VARCHAR(50),
    IN p_modificado_por    VARCHAR(100)
)
BEGIN
    UPDATE meta_ahorro
    SET
        nombre = p_nombre,
        descripcion = p_descripcion,
        monto_total = p_monto_total,
        monto_ahorrado = p_monto_ahorrado,
        fecha_objetivo = p_fecha_objetivo,
        prioridad = p_prioridad,
        estado = p_estado,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_meta_ahorro = p_id_meta_ahorro;
END$$
DELIMITER ;

-- Procedimiento: sp_obligacion_fija_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obligacion_fija_create`(
    IN p_id_usuario         INT,
    IN p_id_subcategoria    INT,
    IN p_nombre_obligacion  VARCHAR(150),
    IN p_descripcion        VARCHAR(255),
    IN p_monto_fijo         DECIMAL(10,2),
    IN p_dia_vencimiento    INT,
    IN p_estado             VARCHAR(50),
    IN p_fecha_inicio       DATE,
    IN p_fecha_finalizacion DATE,
    IN p_creado_por         VARCHAR(100)
)
BEGIN
    INSERT INTO obligacion_fija(
        id_usuario, id_subcategoria, nombre_obligacion,
        descripcion, monto_fijo_mensual, dia_vencimiento,
        estado, fecha_inicio, fecha_finalizacion,
        creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_id_subcategoria, p_nombre_obligacion,
        p_descripcion, p_monto_fijo, p_dia_vencimiento,
        p_estado, p_fecha_inicio, p_fecha_finalizacion,
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_obligacion_fija_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obligacion_fija_delete`(
    IN p_id_obligacion_fija INT
)
BEGIN
    DELETE FROM obligacion_fija
    WHERE id_obligacion_fija = p_id_obligacion_fija;
END$$
DELIMITER ;

-- Procedimiento: sp_obligacion_fija_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obligacion_fija_get`(
    IN p_id_obligacion_fija INT
)
BEGIN
    SELECT *
    FROM obligacion_fija
    WHERE id_obligacion_fija = p_id_obligacion_fija;
END$$
DELIMITER ;

-- Procedimiento: sp_obligacion_fija_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obligacion_fija_list`()
BEGIN
    SELECT *
    FROM obligacion_fija;
END$$
DELIMITER ;

-- Procedimiento: sp_obligacion_fija_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obligacion_fija_update`(
    IN p_id_obligacion_fija INT,
    IN p_nombre_obligacion  VARCHAR(150),
    IN p_descripcion        VARCHAR(255),
    IN p_monto_fijo         DECIMAL(10,2),
    IN p_dia_vencimiento    INT,
    IN p_estado             VARCHAR(50),
    IN p_fecha_finalizacion DATE,
    IN p_modificado_por     VARCHAR(100)
)
BEGIN
    UPDATE obligacion_fija
    SET
        nombre_obligacion = p_nombre_obligacion,
        descripcion = p_descripcion,
        monto_fijo_mensual = p_monto_fijo,
        dia_vencimiento = p_dia_vencimiento,
        estado = p_estado,
        fecha_finalizacion = p_fecha_finalizacion,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_obligacion_fija = p_id_obligacion_fija;
END$$
DELIMITER ;

-- Procedimiento: sp_obtener_resumen_categoria_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_obtener_resumen_categoria_mes`(
    IN p_id_categoria INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    SELECT
        c.nombre_categoria,
        SUM(pd.monto_mensual) AS monto_presupuestado,
        SUM(t.monto) AS monto_ejecutado,
        (SUM(t.monto) / SUM(pd.monto_mensual)) * 100 AS porcentaje
    FROM categoria c
    JOIN subcategoria sc ON sc.id_categoria = c.id_categoria
    JOIN presupuesto_detalle pd ON pd.id_subcategoria = sc.id_subcategoria
    LEFT JOIN transaccion t ON t.id_subcategoria = sc.id_subcategoria
         AND YEAR(t.fecha) = p_anio
         AND MONTH(t.fecha) = p_mes
    WHERE c.id_categoria = p_id_categoria
    GROUP BY c.id_categoria;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_create`(
    IN p_id_usuario        INT,
    IN p_nombre_presupuesto VARCHAR(150),
    IN p_anio_inicio       INT,
    IN p_mes_inicio        INT,
    IN p_anio_fin          INT,
    IN p_mes_fin           INT,
    IN p_total_ingresos    DECIMAL(10,2),
    IN p_total_gastos      DECIMAL(10,2),
    IN p_total_ahorro      DECIMAL(10,2),
    IN p_estado            VARCHAR(50),
    IN p_creado_por        VARCHAR(100)
)
BEGIN
    INSERT INTO presupuesto(
        id_usuario, nombre_presupuesto,
        anio_inicio, mes_inicio,
        anio_fin, mes_fin,
        total_ingresos, total_gastos, total_ahorro,
        fecha_hora_creacion, estado,
        creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_nombre_presupuesto,
        p_anio_inicio, p_mes_inicio,
        p_anio_fin, p_mes_fin,
        p_total_ingresos, p_total_gastos, p_total_ahorro,
        NOW(), p_estado,
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_delete`(
    IN p_id_presupuesto INT
)
BEGIN
    DELETE FROM presupuesto
    WHERE id_presupuesto = p_id_presupuesto;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_detalle_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_detalle_create`(
    IN p_id_presupuesto     INT,
    IN p_id_subcategoria    INT,
    IN p_monto_mensual      DECIMAL(10,2),
    IN p_observacion        VARCHAR(255),
    IN p_creado_por         VARCHAR(100)
)
BEGIN
    INSERT INTO presupuesto_detalle(
        id_presupuesto, id_subcategoria, monto_mensual,
        observacion, creado_por, creado_en
    )
    VALUES(
        p_id_presupuesto, p_id_subcategoria, p_monto_mensual,
        p_observacion, p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_detalle_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_detalle_delete`(
    IN p_id_presupuesto_detalle INT
)
BEGIN
    DELETE FROM presupuesto_detalle
    WHERE id_presupuesto_detalle = p_id_presupuesto_detalle;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_detalle_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_detalle_get`(
    IN p_id_presupuesto_detalle INT
)
BEGIN
    SELECT *
    FROM presupuesto_detalle
    WHERE id_presupuesto_detalle = p_id_presupuesto_detalle;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_detalle_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_detalle_list`()
BEGIN
    SELECT *
    FROM presupuesto_detalle;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_detalle_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_detalle_update`(
    IN p_id_presupuesto_detalle  INT,
    IN p_monto_mensual           DECIMAL(10,2),
    IN p_observacion             VARCHAR(255),
    IN p_modificado_por          VARCHAR(100)
)
BEGIN
    UPDATE presupuesto_detalle
    SET
        monto_mensual = p_monto_mensual,
        observacion = p_observacion,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_presupuesto_detalle = p_id_presupuesto_detalle;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_get`(
    IN p_id_presupuesto INT
)
BEGIN
    SELECT *
    FROM presupuesto
    WHERE id_presupuesto = p_id_presupuesto;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_list`()
BEGIN
    SELECT *
    FROM presupuesto;
END$$
DELIMITER ;

-- Procedimiento: sp_presupuesto_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_presupuesto_update`(
    IN p_id_presupuesto    INT,
    IN p_nombre_presupuesto VARCHAR(150),
    IN p_anio_inicio       INT,
    IN p_mes_inicio        INT,
    IN p_anio_fin          INT,
    IN p_mes_fin           INT,
    IN p_total_ingresos    DECIMAL(10,2),
    IN p_total_gastos      DECIMAL(10,2),
    IN p_total_ahorro      DECIMAL(10,2),
    IN p_estado            VARCHAR(50),
    IN p_modificado_por    VARCHAR(100)
)
BEGIN
    UPDATE presupuesto
    SET
        nombre_presupuesto = p_nombre_presupuesto,
        anio_inicio = p_anio_inicio,
        mes_inicio = p_mes_inicio,
        anio_fin = p_anio_fin,
        mes_fin = p_mes_fin,
        total_ingresos = p_total_ingresos,
        total_gastos = p_total_gastos,
        total_ahorro = p_total_ahorro,
        estado = p_estado,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_presupuesto = p_id_presupuesto;
END$$
DELIMITER ;

-- Procedimiento: sp_procesar_obligaciones_mes
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_procesar_obligaciones_mes`(
    IN p_id_usuario INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    SELECT *
    FROM obligacion_fija
    WHERE id_usuario = p_id_usuario
      AND estado = 'activa'
      AND MONTH(fecha_inicio) <= p_mes
      AND (fecha_finalizacion IS NULL OR MONTH(fecha_finalizacion) >= p_mes);
END$$
DELIMITER ;

-- Procedimiento: sp_registrar_transaccion_completa
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_registrar_transaccion_completa`(
    IN p_id_usuario INT,
    IN p_id_presupuesto INT,
    IN p_id_subcategoria INT,
    IN p_id_obligacion_fija INT,
    IN p_tipo_transaccion VARCHAR(50),
    IN p_descripcion VARCHAR(255),
    IN p_monto DECIMAL(10,2),
    IN p_fecha DATE,
    IN p_metodo_pago VARCHAR(100),
    IN p_numero_factura VARCHAR(100),
    IN p_observaciones VARCHAR(255),
    IN p_creado_por VARCHAR(100)
)
BEGIN
    INSERT INTO transaccion(
        id_usuario, id_presupuesto, id_subcategoria, id_obligacion_fija,
        tipo_transaccion, descripcion, monto, fecha, metodo_pago,
        numero_factura, observaciones, fecha_hora_registro,
        creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_id_presupuesto, p_id_subcategoria, p_id_obligacion_fija,
        p_tipo_transaccion, p_descripcion, p_monto, p_fecha, p_metodo_pago,
        p_numero_factura, p_observaciones, NOW(),
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_subcategoria_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_subcategoria_create`(
    IN p_id_categoria         INT,
    IN p_nombre_subcategoria  VARCHAR(100),
    IN p_estado               VARCHAR(50),
    IN p_auto                 TINYINT,
    IN p_creado_por           VARCHAR(100)
)
BEGIN
    INSERT INTO subcategoria(
        id_categoria, nombre_subcategoria, estado, auto,
        creado_por, creado_en
    )
    VALUES(
        p_id_categoria, p_nombre_subcategoria, p_estado, p_auto,
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_subcategoria_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_subcategoria_delete`(
    IN p_id_subcategoria INT
)
BEGIN
    DELETE FROM subcategoria
    WHERE id_subcategoria = p_id_subcategoria;
END$$
DELIMITER ;

-- Procedimiento: sp_subcategoria_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_subcategoria_get`(
    IN p_id_subcategoria INT
)
BEGIN
    SELECT *
    FROM subcategoria
    WHERE id_subcategoria = p_id_subcategoria;
END$$
DELIMITER ;

-- Procedimiento: sp_subcategoria_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_subcategoria_list`()
BEGIN
    SELECT *
    FROM subcategoria;
END$$
DELIMITER ;

-- Procedimiento: sp_subcategoria_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_subcategoria_update`(
    IN p_id_subcategoria      INT,
    IN p_nombre_subcategoria  VARCHAR(100),
    IN p_estado               VARCHAR(50),
    IN p_auto                 TINYINT,
    IN p_modificado_por       VARCHAR(100)
)
BEGIN
    UPDATE subcategoria
    SET
        nombre_subcategoria = p_nombre_subcategoria,
        estado = p_estado,
        auto = p_auto,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_subcategoria = p_id_subcategoria;
END$$
DELIMITER ;

-- Procedimiento: sp_transaccion_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_transaccion_create`(
    IN p_id_usuario         INT,
    IN p_id_presupuesto     INT,
    IN p_id_subcategoria    INT,
    IN p_id_obligacion_fija INT,
    IN p_tipo_transaccion   VARCHAR(50),
    IN p_descripcion        VARCHAR(255),
    IN p_monto              DECIMAL(10,2),
    IN p_fecha              DATE,
    IN p_metodo_pago        VARCHAR(100),
    IN p_numero_factura     VARCHAR(100),
    IN p_observaciones      VARCHAR(255),
    IN p_creado_por         VARCHAR(100)
)
BEGIN
    INSERT INTO transaccion(
        id_usuario, id_presupuesto, id_subcategoria, id_obligacion_fija,
        tipo_transaccion, descripcion, monto, fecha, metodo_pago,
        numero_factura, observaciones, fecha_hora_registro,
        creado_por, creado_en
    )
    VALUES(
        p_id_usuario, p_id_presupuesto, p_id_subcategoria, p_id_obligacion_fija,
        p_tipo_transaccion, p_descripcion, p_monto, p_fecha, p_metodo_pago,
        p_numero_factura, p_observaciones, NOW(),
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_transaccion_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_transaccion_delete`(
    IN p_id_transaccion INT
)
BEGIN
    DELETE FROM transaccion
    WHERE id_transaccion = p_id_transaccion;
END$$
DELIMITER ;

-- Procedimiento: sp_transaccion_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_transaccion_get`(
    IN p_id_transaccion INT
)
BEGIN
    SELECT *
    FROM transaccion
    WHERE id_transaccion = p_id_transaccion;
END$$
DELIMITER ;

-- Procedimiento: sp_transaccion_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_transaccion_list`()
BEGIN
    SELECT *
    FROM transaccion;
END$$
DELIMITER ;

-- Procedimiento: sp_transaccion_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_transaccion_update`(
    IN p_id_transaccion     INT,
    IN p_descripcion        VARCHAR(255),
    IN p_monto              DECIMAL(10,2),
    IN p_fecha              DATE,
    IN p_metodo_pago        VARCHAR(100),
    IN p_numero_factura     VARCHAR(100),
    IN p_observaciones      VARCHAR(255),
    IN p_modificado_por     VARCHAR(100)
)
BEGIN
    UPDATE transaccion
    SET
        descripcion = p_descripcion,
        monto = p_monto,
        fecha = p_fecha,
        metodo_pago = p_metodo_pago,
        numero_factura = p_numero_factura,
        observaciones = p_observaciones,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_transaccion = p_id_transaccion;
END$$
DELIMITER ;

-- Procedimiento: sp_usuario_create
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_usuario_create`(
    IN p_primer_nombre      VARCHAR(100),
    IN p_segundo_nombre     VARCHAR(100),
    IN p_primer_apellido    VARCHAR(100),
    IN p_segundo_apellido   VARCHAR(100),
    IN p_correo             VARCHAR(150),
    IN p_salario_mensual    DECIMAL(10,2),
    IN p_creado_por         VARCHAR(100)
)
BEGIN
    INSERT INTO usuario(
        primer_nombre, segundo_nombre, primer_apellido, segundo_apellido,
        correo, fecha_registro, salario_mensual_base, estado,
        creado_por, creado_en
    )
    VALUES(
        p_primer_nombre, p_segundo_nombre, p_primer_apellido, p_segundo_apellido,
        p_correo, NOW(), p_salario_mensual, 'activo',
        p_creado_por, NOW()
    );
END$$
DELIMITER ;

-- Procedimiento: sp_usuario_delete
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_usuario_delete`(
    IN p_id_usuario     INT,
    IN p_modificado_por VARCHAR(100)
)
BEGIN
    UPDATE usuario
    SET
        estado = 'inactivo',
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_usuario = p_id_usuario;
END$$
DELIMITER ;

-- Procedimiento: sp_usuario_get
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_usuario_get`(
    IN p_id_usuario INT
)
BEGIN
    SELECT *
    FROM usuario
    WHERE id_usuario = p_id_usuario;
END$$
DELIMITER ;

-- Procedimiento: sp_usuario_list
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_usuario_list`()
BEGIN
    SELECT *
    FROM usuario;
END$$
DELIMITER ;

-- Procedimiento: sp_usuario_update
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_usuario_update`(
    IN p_id_usuario         INT,
    IN p_primer_nombre      VARCHAR(100),
    IN p_segundo_nombre     VARCHAR(100),
    IN p_primer_apellido    VARCHAR(100),
    IN p_segundo_apellido   VARCHAR(100),
    IN p_correo             VARCHAR(150),
    IN p_salario_mensual    DECIMAL(10,2),
    IN p_modificado_por     VARCHAR(100)
)
BEGIN
    UPDATE usuario
    SET
        primer_nombre = p_primer_nombre,
        segundo_nombre = p_segundo_nombre,
        primer_apellido = p_primer_apellido,
        segundo_apellido = p_segundo_apellido,
        correo = p_correo,
        salario_mensual_base = p_salario_mensual,
        modificado_por = p_modificado_por,
        modificado_en = NOW()
    WHERE id_usuario = p_id_usuario;
END$$
DELIMITER ;

-- Procedimiento: sp_verificar_generar_alertas_presupuesto
DELIMITER $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_verificar_generar_alertas_presupuesto`(
    IN p_id_presupuesto INT,
    IN p_anio INT,
    IN p_mes INT
)
BEGIN
    SELECT
        sc.nombre_subcategoria,
        pd.monto_mensual AS presupuestado,
        SUM(t.monto) AS ejecutado,
        (SUM(t.monto) / pd.monto_mensual) * 100 AS porcentaje
    FROM presupuesto_detalle pd
    LEFT JOIN transaccion t
        ON pd.id_subcategoria = t.id_subcategoria
        AND YEAR(t.fecha) = p_anio
        AND MONTH(t.fecha) = p_mes
    JOIN subcategoria sc ON sc.id_subcategoria = pd.id_subcategoria
    WHERE pd.id_presupuesto = p_id_presupuesto
    GROUP BY pd.id_subcategoria;
END$$
DELIMITER ;

-- Trigger: trg_categoria_crear_subcategoria
DELIMITER $$
CREATE DEFINER=`root`@`localhost` TRIGGER trg_categoria_crear_subcategoria
AFTER INSERT ON categoria
FOR EACH ROW
BEGIN
    INSERT INTO subcategoria (
        id_categoria,
        nombre_subcategoria,
        estado,
        auto,
        creado_por,
        creado_en
    )
    VALUES (
        NEW.id_categoria,
        'General',
        'activo',
        1,
        NEW.creado_por,
        NOW()
    );
END$$
DELIMITER ;

-- Trigger: trg_alerta_meta_ahorro
DELIMITER $$
CREATE DEFINER=`root`@`localhost` TRIGGER trg_alerta_meta_ahorro
AFTER UPDATE ON meta_ahorro
FOR EACH ROW
BEGIN
    DECLARE v_porcentaje DECIMAL(5,2);

    -- Calcular porcentaje de avance
    SET v_porcentaje = (NEW.monto_ahorrado / NEW.monto_total) * 100;

    -- Alerta cuando alcanza el 50%
    IF v_porcentaje >= 50
       AND (OLD.monto_ahorrado / OLD.monto_total) * 100 < 50 THEN

        INSERT INTO alerta (
            id_usuario,
            tipo,
            prioridad,
            mensaje,
            creado_en,
            vista
        )
        VALUES (
            NEW.id_usuario,
            'meta_50',
            'informativa',
            CONCAT('Has alcanzado el 50% de la meta: ', NEW.nombre),
            NOW(),
            0
        );
    END IF;

    -- Cuando se completa la meta (100%)
    IF v_porcentaje >= 100
       AND OLD.estado <> 'completada' THEN

        UPDATE meta_ahorro
        SET estado = 'completada'
        WHERE id_meta_ahorro = NEW.id_meta_ahorro;

        INSERT INTO alerta (
            id_usuario,
            tipo,
            prioridad,
            mensaje,
            creado_en,
            vista
        )
        VALUES (
            NEW.id_usuario,
            'meta_completada',
            'informativa',
            CONCAT('Meta completada: ', NEW.nombre),
            NOW(),
            0
        );
    END IF;

END$$
DELIMITER ;

-- Trigger: trg_actualizar_meta_ahorro
DELIMITER $$
CREATE DEFINER=`root`@`localhost` TRIGGER trg_actualizar_meta_ahorro
AFTER INSERT ON transaccion
FOR EACH ROW
BEGIN
    DECLARE v_tipo_categoria VARCHAR(100);
    DECLARE v_id_meta INT;

    -- Obtener el tipo real de la categoría
    SELECT c.tipo_categoria
    INTO v_tipo_categoria
    FROM subcategoria sc
    INNER JOIN categoria c 
        ON sc.id_categoria = c.id_categoria
    WHERE sc.id_subcategoria = NEW.id_subcategoria
    LIMIT 1;

    -- Si la categoría es de ahorro
    IF v_tipo_categoria = 'ahorro' THEN

        SELECT id_meta_ahorro
        INTO v_id_meta
        FROM meta_ahorro
        WHERE id_subcategoria = NEW.id_subcategoria
          AND estado = 'en_progreso'
        LIMIT 1;

        IF v_id_meta IS NOT NULL THEN
            UPDATE meta_ahorro
            SET monto_ahorrado = monto_ahorrado + NEW.monto
            WHERE id_meta_ahorro = v_id_meta;
        END IF;

    END IF;
END$$
DELIMITER ;

-- Trigger: trg_alerta_presupuesto
DELIMITER $$
CREATE DEFINER=`root`@`localhost` TRIGGER trg_alerta_presupuesto
AFTER INSERT ON transaccion
FOR EACH ROW
BEGIN
    DECLARE v_presupuestado DECIMAL(10,2);
    DECLARE v_ejecutado DECIMAL(10,2);
    DECLARE v_porcentaje DECIMAL(5,2);
    DECLARE v_tipo_categoria VARCHAR(100);

    -- Obtener tipo REAL desde la categoría padre
    SELECT c.tipo_categoria
    INTO v_tipo_categoria
    FROM subcategoria sc
    INNER JOIN categoria c 
        ON sc.id_categoria = c.id_categoria
    WHERE sc.id_subcategoria = NEW.id_subcategoria
    LIMIT 1;

    -- Solo aplica a gastos
    IF v_tipo_categoria = 'gasto' THEN

        SELECT monto_mensual
        INTO v_presupuestado
        FROM presupuesto_detalle
        WHERE id_presupuesto = NEW.id_presupuesto
          AND id_subcategoria = NEW.id_subcategoria
        LIMIT 1;

        SELECT SUM(monto)
        INTO v_ejecutado
        FROM transaccion
        WHERE id_presupuesto = NEW.id_presupuesto
          AND id_subcategoria = NEW.id_subcategoria;

        IF v_presupuestado IS NOT NULL AND v_ejecutado IS NOT NULL THEN
            SET v_porcentaje = (v_ejecutado / v_presupuestado) * 100;
            -- Aquí solo se evalúa el porcentaje (las alertas se generan en reportes)
        END IF;

    END IF;
END$$
DELIMITER ;

