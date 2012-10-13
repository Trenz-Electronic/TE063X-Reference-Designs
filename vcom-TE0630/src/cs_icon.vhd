-------------------------------------------------------------------------------
-- Copyright (c) 2012 Xilinx, Inc.
-- All Rights Reserved
-------------------------------------------------------------------------------
--   ____  ____
--  /   /\/   /
-- /___/  \  /    Vendor     : Xilinx
-- \   \   \/     Version    : 14.2
--  \   \         Application: XILINX CORE Generator
--  /   /         Filename   : cs_icon.vhd
-- /___/   /\     Timestamp  : Mon Sep 03 22:58:05 FLE Daylight Time 2012
-- \   \  /  \
--  \___\/\___\
--
-- Design Name: VHDL Synthesis Wrapper
-------------------------------------------------------------------------------
-- This wrapper is used to integrate with Project Navigator and PlanAhead

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
ENTITY cs_icon IS
  port (
    CONTROL0: inout std_logic_vector(35 downto 0));
END cs_icon;

ARCHITECTURE cs_icon_a OF cs_icon IS
BEGIN

END cs_icon_a;
