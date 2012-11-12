------------------------------------------------------------------------------
-- axi_fx2.vhd - entity/architecture pair
------------------------------------------------------------------------------
-- IMPORTANT:
-- DO NOT MODIFY THIS FILE EXCEPT IN THE DESIGNATED SECTIONS.
--
-- SEARCH FOR --USER TO DETERMINE WHERE CHANGES ARE ALLOWED.
--
-- TYPICALLY, THE ONLY ACCEPTABLE CHANGES INVOLVE ADDING NEW
-- PORTS AND GENERICS THAT GET PASSED THROUGH TO THE INSTANTIATION
-- OF THE USER_LOGIC ENTITY.
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          axi_fx2.vhd
-- Version:           1.00.a
-- Description:       Top level design, instantiates library components and user logic.
-- Date:              Mon Jun 25 15:20:37 2012 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

library ieee;
use ieee.STD_LOGIC_1164.all;
use ieee.STD_LOGIC_arith.all;
use ieee.STD_LOGIC_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;
use proc_common_v3_00_a.ipif_pkg.all;
use proc_common_v3_00_a.soft_reset;

library axi_lite_ipif_v1_01_a;
use axi_lite_ipif_v1_01_a.axi_lite_ipif;

library axi_fx2_v1_00_a;
use axi_fx2_v1_00_a.user_logic;

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_S_AXI_DATA_WIDTH           -- 
--   C_S_AXI_ADDR_WIDTH           -- 
--   C_S_AXI_MIN_SIZE             -- 
--   C_USE_WSTRB                  -- 
--   C_DPHASE_TIMEOUT             -- 
--   C_BASEADDR                   -- AXI4LITE slave: base address
--   C_HIGHADDR                   -- AXI4LITE slave: high address
--   C_FAMILY                     -- 
--   C_NUM_REG                    -- Number of software accessible registers
--   C_NUM_MEM                    -- Number of address-ranges
--   C_SLV_AWIDTH                 -- Slave interface address bus width
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   S_AXI_ACLK                   -- 
--   S_AXI_ARESETN                -- 
--   S_AXI_AWADDR                 -- 
--   S_AXI_AWVALID                -- 
--   S_AXI_WDATA                  -- 
--   S_AXI_WSTRB                  -- 
--   S_AXI_WVALID                 -- 
--   S_AXI_BREADY                 -- 
--   S_AXI_ARADDR                 -- 
--   S_AXI_ARVALID                -- 
--   S_AXI_RREADY                 -- 
--   S_AXI_ARREADY                -- 
--   S_AXI_RDATA                  -- 
--   S_AXI_RRESP                  -- 
--   S_AXI_RVALID                 -- 
--   S_AXI_WREADY                 -- 
--   S_AXI_BRESP                  -- 
--   S_AXI_BVALID                 -- 
--   S_AXI_AWREADY                -- 
------------------------------------------------------------------------------

entity axi_fx2 is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_S_AXI_DATA_WIDTH             : integer              := 32;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_S_AXI_MIN_SIZE               : STD_LOGIC_VECTOR     := X"000001FF";
    C_USE_WSTRB                    : integer              := 0;
    C_DPHASE_TIMEOUT               : integer              := 8;
    C_BASEADDR                     : STD_LOGIC_VECTOR     := X"FFFFFFFF";
    C_HIGHADDR                     : STD_LOGIC_VECTOR     := X"00000000";
    C_FAMILY                       : string               := "virtex6";
    C_NUM_REG                      : integer              := 1;
    C_NUM_MEM                      : integer              := 1;
    C_SLV_AWIDTH                   : integer              := 32;
    C_SLV_DWIDTH                   : integer              := 32
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
    -- ADD USER PORTS ABOVE THIS LINE ------------------
	USB_IFCLK           			: in  STD_LOGIC;
    USB_SLRD            			: out STD_LOGIC;
    USB_SLWR            			: out STD_LOGIC;
    USB_FLAGA           			: in  STD_LOGIC;
    USB_FLAGB           			: in  STD_LOGIC;
    USB_FLAGC           			: in  STD_LOGIC;
	USB_FLAGD           			: in  STD_LOGIC;
	USB_SLOE            			: out STD_LOGIC;
	USB_PKTEND          			: out STD_LOGIC;
	USB_FIFOADR         			: out STD_LOGIC_VECTOR(1 downto 0); --"00"=EP2,"01"=EP4,"10"=EP6,11"=EP8
    USB_FD_T            			: out STD_LOGIC_VECTOR(7 downto 0) := (others => '1'); --OE active low
    USB_FD_O            			: out STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
    USB_FD_I            			: in  STD_LOGIC_VECTOR(7 downto 0);
	
	S_AXIS_ACLK						: in  STD_LOGIC;
	S_AXIS_TREADY					: out STD_LOGIC;
	S_AXIS_TDATA					: in  STD_LOGIC_VECTOR(31 downto 0);
	S_AXIS_TKEEP					: in  STD_LOGIC_VECTOR( 3 downto 0);
	S_AXIS_TLAST					: in  STD_LOGIC;
	S_AXIS_TVALID					: in  STD_LOGIC;
	
	M_AXIS_ACLK						: in  STD_LOGIC;
	M_AXIS_TVALID					: out STD_LOGIC;
	M_AXIS_TDATA					: out STD_LOGIC_VECTOR(31 downto 0);
	M_AXIS_TKEEP					: out STD_LOGIC_VECTOR( 3 downto 0);
	M_AXIS_TLAST					: out STD_LOGIC;
	M_AXIS_TREADY					: in  STD_LOGIC;
	
	C_AXIS_ACLK						: in  STD_LOGIC;
	C_AXIS_TVALID					: in  STD_LOGIC;
	C_AXIS_TREADY					: out STD_LOGIC;
	C_AXIS_TLAST					: in  STD_LOGIC;
	C_AXIS_TKEEP					: in  STD_LOGIC_VECTOR( 3 downto 0);
	C_AXIS_TDATA					: in  STD_LOGIC_VECTOR(31 downto 0);

	CHIPSCOPE						: out STD_LOGIC_VECTOR(63 downto 0);
	dbgin							: in  STD_LOGIC;
	
    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    S_AXI_ACLK                     : in  STD_LOGIC;
    S_AXI_ARESETN                  : in  STD_LOGIC;
    S_AXI_AWADDR                   : in  STD_LOGIC_VECTOR(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_AWVALID                  : in  STD_LOGIC;
    S_AXI_WDATA                    : in  STD_LOGIC_VECTOR(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_WSTRB                    : in  STD_LOGIC_VECTOR((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
    S_AXI_WVALID                   : in  STD_LOGIC;
    S_AXI_BREADY                   : in  STD_LOGIC;
    S_AXI_ARADDR                   : in  STD_LOGIC_VECTOR(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_ARVALID                  : in  STD_LOGIC;
    S_AXI_RREADY                   : in  STD_LOGIC;
    S_AXI_ARREADY                  : out STD_LOGIC;
    S_AXI_RDATA                    : out STD_LOGIC_VECTOR(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_RRESP                    : out STD_LOGIC_VECTOR(1 downto 0);
    S_AXI_RVALID                   : out STD_LOGIC;
    S_AXI_WREADY                   : out STD_LOGIC;
    S_AXI_BRESP                    : out STD_LOGIC_VECTOR(1 downto 0);
    S_AXI_BVALID                   : out STD_LOGIC;
    S_AXI_AWREADY                  : out STD_LOGIC
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;
  attribute MAX_FANOUT of S_AXI_ACLK	: signal is "10000";
  attribute MAX_FANOUT of S_AXI_ARESETN	: signal is "10000";
  attribute SIGIS of S_AXI_ACLK			: signal is "Clk";
  attribute SIGIS of S_AXI_ARESETN		: signal is "Rst";
end entity axi_fx2;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of axi_fx2 is

  constant USER_SLV_DWIDTH      : integer              := C_S_AXI_DATA_WIDTH;
  constant IPIF_SLV_DWIDTH      : integer              := C_S_AXI_DATA_WIDTH;
  constant ZERO_ADDR_PAD        : STD_LOGIC_VECTOR(0 to 31) := (others => '0');
  constant RST_BASEADDR         : STD_LOGIC_VECTOR     := C_BASEADDR or X"00000100";
  constant RST_HIGHADDR         : STD_LOGIC_VECTOR     := C_BASEADDR or X"000001FF";
  constant USER_SLV_BASEADDR	: STD_LOGIC_VECTOR     := C_BASEADDR or X"00000000";
  constant USER_SLV_HIGHADDR    : STD_LOGIC_VECTOR     := C_BASEADDR or X"000000FF";

  constant IPIF_ARD_ADDR_RANGE_ARRAY      : SLV64_ARRAY_TYPE     := 
    (
      ZERO_ADDR_PAD & RST_BASEADDR,       -- soft reset space base address
      ZERO_ADDR_PAD & RST_HIGHADDR,       -- soft reset space high address
      ZERO_ADDR_PAD & USER_SLV_BASEADDR,  -- user logic slave space base address
      ZERO_ADDR_PAD & USER_SLV_HIGHADDR   -- user logic slave space high address
    );

  constant RST_NUM_CE           : integer              := 1;
  constant USER_SLV_NUM_REG     : integer              := 8;
  constant USER_NUM_REG         : integer              := USER_SLV_NUM_REG;
  constant TOTAL_IPIF_CE        : integer              := USER_NUM_REG + RST_NUM_CE;

  constant IPIF_ARD_NUM_CE_ARRAY          : INTEGER_ARRAY_TYPE   := 
    (
      0  => (RST_NUM_CE),                 -- number of ce for soft reset space
      1  => (USER_SLV_NUM_REG)            -- number of ce for user logic slave space
    );

  ------------------------------------------
  -- Width of triggered reset in bus clocks
  ------------------------------------------
  constant RESET_WIDTH                    : integer              := 8;

  ------------------------------------------
  -- Index for CS/CE
  ------------------------------------------
  constant RST_CS_INDEX                   : integer              := 0;
  constant RST_CE_INDEX                   : integer              := USER_NUM_REG;
  constant USER_SLV_CS_INDEX              : integer              := 1;
  constant USER_SLV_CE_INDEX              : integer              := calc_start_ce_index(IPIF_ARD_NUM_CE_ARRAY, USER_SLV_CS_INDEX);
  constant USER_CE_INDEX                  : integer              := USER_SLV_CE_INDEX;

  ------------------------------------------
  -- IP Interconnect (IPIC) signal declarations
  ------------------------------------------
  signal ipif_Bus2IP_Clk                : STD_LOGIC;
  signal ipif_Bus2IP_Resetn             : STD_LOGIC;
  signal ipif_Bus2IP_Addr               : STD_LOGIC_VECTOR(C_S_AXI_ADDR_WIDTH-1 downto 0);
  signal ipif_Bus2IP_RNW                : STD_LOGIC;
  signal ipif_Bus2IP_BE                 : STD_LOGIC_VECTOR(IPIF_SLV_DWIDTH/8-1 downto 0);
  signal ipif_Bus2IP_CS                 : STD_LOGIC_VECTOR((IPIF_ARD_ADDR_RANGE_ARRAY'LENGTH)/2-1 downto 0);
  signal ipif_Bus2IP_RdCE               : STD_LOGIC_VECTOR(calc_num_ce(IPIF_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_Bus2IP_WrCE               : STD_LOGIC_VECTOR(calc_num_ce(IPIF_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_Bus2IP_Data               : STD_LOGIC_VECTOR(IPIF_SLV_DWIDTH-1 downto 0);
  signal ipif_IP2Bus_WrAck              : STD_LOGIC;
  signal ipif_IP2Bus_RdAck              : STD_LOGIC;
  signal ipif_IP2Bus_Error              : STD_LOGIC;
  signal ipif_IP2Bus_Data               : STD_LOGIC_VECTOR(IPIF_SLV_DWIDTH-1 downto 0);
  signal ipif_Bus2IP_Reset              : STD_LOGIC;
  signal rst_Bus2IP_Reset               : STD_LOGIC;
  signal rst_IP2Bus_WrAck               : STD_LOGIC;
  signal rst_IP2Bus_Error               : STD_LOGIC;
  signal rst_Bus2IP_Reset_tmp           : STD_LOGIC;
  signal user_Bus2IP_RdCE               : STD_LOGIC_VECTOR(USER_NUM_REG-1 downto 0);
  signal user_Bus2IP_WrCE               : STD_LOGIC_VECTOR(USER_NUM_REG-1 downto 0);
  signal user_IP2Bus_Data               : STD_LOGIC_VECTOR(USER_SLV_DWIDTH-1 downto 0);
  signal user_IP2Bus_RdAck              : STD_LOGIC;
  signal user_IP2Bus_WrAck              : STD_LOGIC;
  signal user_IP2Bus_Error              : STD_LOGIC;

begin

  ------------------------------------------
  -- instantiate axi_lite_ipif
  ------------------------------------------
  AXI_LITE_IPIF_I : entity axi_lite_ipif_v1_01_a.axi_lite_ipif
    generic map
    (
      C_S_AXI_DATA_WIDTH             => IPIF_SLV_DWIDTH,
      C_S_AXI_ADDR_WIDTH             => C_S_AXI_ADDR_WIDTH,
      C_S_AXI_MIN_SIZE               => C_S_AXI_MIN_SIZE,
      C_USE_WSTRB                    => C_USE_WSTRB,
      C_DPHASE_TIMEOUT               => C_DPHASE_TIMEOUT,
      C_ARD_ADDR_RANGE_ARRAY         => IPIF_ARD_ADDR_RANGE_ARRAY,
      C_ARD_NUM_CE_ARRAY             => IPIF_ARD_NUM_CE_ARRAY,
      C_FAMILY                       => C_FAMILY
    )
    port map
    (
      S_AXI_ACLK                     => S_AXI_ACLK,
      S_AXI_ARESETN                  => S_AXI_ARESETN,
      S_AXI_AWADDR                   => S_AXI_AWADDR,
      S_AXI_AWVALID                  => S_AXI_AWVALID,
      S_AXI_WDATA                    => S_AXI_WDATA,
      S_AXI_WSTRB                    => S_AXI_WSTRB,
      S_AXI_WVALID                   => S_AXI_WVALID,
      S_AXI_BREADY                   => S_AXI_BREADY,
      S_AXI_ARADDR                   => S_AXI_ARADDR,
      S_AXI_ARVALID                  => S_AXI_ARVALID,
      S_AXI_RREADY                   => S_AXI_RREADY,
      S_AXI_ARREADY                  => S_AXI_ARREADY,
      S_AXI_RDATA                    => S_AXI_RDATA,
      S_AXI_RRESP                    => S_AXI_RRESP,
      S_AXI_RVALID                   => S_AXI_RVALID,
      S_AXI_WREADY                   => S_AXI_WREADY,
      S_AXI_BRESP                    => S_AXI_BRESP,
      S_AXI_BVALID                   => S_AXI_BVALID,
      S_AXI_AWREADY                  => S_AXI_AWREADY,
      Bus2IP_Clk                     => ipif_Bus2IP_Clk,
      Bus2IP_Resetn                  => ipif_Bus2IP_Resetn,
      Bus2IP_Addr                    => ipif_Bus2IP_Addr,
      Bus2IP_RNW                     => ipif_Bus2IP_RNW,
      Bus2IP_BE                      => ipif_Bus2IP_BE,
      Bus2IP_CS                      => ipif_Bus2IP_CS,
      Bus2IP_RdCE                    => ipif_Bus2IP_RdCE,
      Bus2IP_WrCE                    => ipif_Bus2IP_WrCE,
      Bus2IP_Data                    => ipif_Bus2IP_Data,
      IP2Bus_WrAck                   => ipif_IP2Bus_WrAck,
      IP2Bus_RdAck                   => ipif_IP2Bus_RdAck,
      IP2Bus_Error                   => ipif_IP2Bus_Error,
      IP2Bus_Data                    => ipif_IP2Bus_Data
    );

  ------------------------------------------
  -- instantiate soft_reset
  ------------------------------------------
  SOFT_RESET_I : entity proc_common_v3_00_a.soft_reset
    generic map
    (
      C_SIPIF_DWIDTH                 => IPIF_SLV_DWIDTH,
      C_RESET_WIDTH                  => RESET_WIDTH
    )
    port map
    (
      Bus2IP_Reset                   => ipif_Bus2IP_Reset,
      Bus2IP_Clk                     => ipif_Bus2IP_Clk,
      Bus2IP_WrCE                    => ipif_Bus2IP_WrCE(RST_CE_INDEX),
      Bus2IP_Data                    => ipif_Bus2IP_Data,
      Bus2IP_BE                      => ipif_Bus2IP_BE,
      Reset2IP_Reset                 => rst_Bus2IP_Reset,
      Reset2Bus_WrAck                => rst_IP2Bus_WrAck,
      Reset2Bus_Error                => rst_IP2Bus_Error,
      Reset2Bus_ToutSup              => open
    );

  ------------------------------------------
  -- instantiate User Logic
  ------------------------------------------
  USER_LOGIC_I : entity axi_fx2_v1_00_a.user_logic
    generic map
    (
      -- MAP USER GENERICS BELOW THIS LINE ---------------
      --USER generics mapped here
      -- MAP USER GENERICS ABOVE THIS LINE ---------------

      C_NUM_REG                      => USER_NUM_REG,
      C_SLV_DWIDTH                   => USER_SLV_DWIDTH
    )
    port map
    (
      -- MAP USER PORTS BELOW THIS LINE ------------------
      --USER ports mapped here
		USB_IFCLK   					=> USB_IFCLK,  
		USB_SLRD						=> USB_SLRD,   
		USB_SLWR						=> USB_SLWR,   
		USB_FLAGA						=> USB_FLAGA,  
		USB_FLAGB						=> USB_FLAGB,  
		USB_FLAGC						=> USB_FLAGC,  
		USB_FLAGD						=> USB_FLAGD,  
		USB_SLOE						=> USB_SLOE,   
		USB_PKTEND						=> USB_PKTEND, 
		USB_FIFOADR						=> USB_FIFOADR,
		USB_FD_T						=> USB_FD_T,   
		USB_FD_O						=> USB_FD_O,   
		USB_FD_I						=> USB_FD_I,	 
		
		S_AXIS_ACLK						=> S_AXIS_ACLK,
		S_AXIS_TREADY					=> S_AXIS_TREADY,
		S_AXIS_TDATA					=> S_AXIS_TDATA,
		S_AXIS_TKEEP					=> S_AXIS_TKEEP,
		S_AXIS_TLAST					=> S_AXIS_TLAST,
		S_AXIS_TVALID					=> S_AXIS_TVALID,
		M_AXIS_ACLK						=> M_AXIS_ACLK,
		M_AXIS_TVALID					=> M_AXIS_TVALID,
		M_AXIS_TDATA					=> M_AXIS_TDATA,
		M_AXIS_TKEEP					=> M_AXIS_TKEEP,
		M_AXIS_TLAST					=> M_AXIS_TLAST,
		M_AXIS_TREADY					=> M_AXIS_TREADY,
		
		CHIPSCOPE						=> CHIPSCOPE,
		dbgin							=> dbgin,
      -- MAP USER PORTS ABOVE THIS LINE ------------------

		Bus2IP_Clk                     => ipif_Bus2IP_Clk,
		Bus2IP_Resetn                  => rst_Bus2IP_Reset_tmp,
		Bus2IP_Addr                    => ipif_Bus2IP_Addr,
		Bus2IP_CS                      => ipif_Bus2IP_CS,
		Bus2IP_RNW                     => ipif_Bus2IP_RNW,
		Bus2IP_Data                    => ipif_Bus2IP_Data,
		Bus2IP_BE                      => ipif_Bus2IP_BE,
		Bus2IP_RdCE                    => user_Bus2IP_RdCE,
		Bus2IP_WrCE                    => user_Bus2IP_WrCE,
		IP2Bus_Data                    => user_IP2Bus_Data,
		IP2Bus_RdAck                   => user_IP2Bus_RdAck,
		IP2Bus_WrAck                   => user_IP2Bus_WrAck,
		IP2Bus_Error                   => user_IP2Bus_Error
    );

  ------------------------------------------
  -- connect internal signals
  ------------------------------------------
  IP2BUS_DATA_MUX_PROC : process( ipif_Bus2IP_CS, user_IP2Bus_Data ) is
  begin

    case ipif_Bus2IP_CS is
      when "01" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "10" => ipif_IP2Bus_Data <= (others => '0');
      when others => ipif_IP2Bus_Data <= (others => '0');
    end case;

  end process IP2BUS_DATA_MUX_PROC;

  ipif_IP2Bus_WrAck 	<= user_IP2Bus_WrAck or rst_IP2Bus_WrAck;
  ipif_IP2Bus_RdAck 	<= user_IP2Bus_RdAck;
  ipif_IP2Bus_Error 	<= user_IP2Bus_Error or rst_IP2Bus_Error;

  user_Bus2IP_RdCE 		<= ipif_Bus2IP_RdCE(USER_NUM_REG-1 downto 0);
  user_Bus2IP_WrCE 		<= ipif_Bus2IP_WrCE(USER_NUM_REG-1 downto 0);

  ipif_Bus2IP_Reset 	<= not ipif_Bus2IP_Resetn;
  rst_Bus2IP_Reset_tmp	<= not rst_Bus2IP_Reset;
  C_AXIS_TREADY			<= '1'; -- Stub
--------------------------------------------------------------------------------
end IMP;
