------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
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
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
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

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.STD_LOGIC_1164.all;
use ieee.STD_LOGIC_arith.all;
use ieee.STD_LOGIC_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_NUM_REG                    -- Number of software accessible registers
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Addr                  -- Bus to IP address bus
--   Bus2IP_CS                    -- Bus to IP chip select
--   Bus2IP_RNW                   -- Bus to IP read/not write
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
	C_DEF_WR_EP						: INTEGER			:= 6;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      	: INTEGER           := 8;
    C_SLV_DWIDTH                   	: INTEGER           := 32
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
	-- AXI Stream
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
	-- ChipScope
	CHIPSCOPE						: out STD_LOGIC_VECTOR(63 downto 0);
	dbgin							: in  STD_LOGIC;
    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  STD_LOGIC;
    Bus2IP_Resetn                  : in  STD_LOGIC;
    Bus2IP_Addr                    : in  STD_LOGIC_VECTOR(0 to 31);
    Bus2IP_CS                      : in  STD_LOGIC_VECTOR(0 to 1);
    Bus2IP_RNW                     : in  STD_LOGIC;
    Bus2IP_Data                    : in  STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  STD_LOGIC_VECTOR(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  STD_LOGIC_VECTOR(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  STD_LOGIC_VECTOR(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out STD_LOGIC;
    IP2Bus_WrAck                   : out STD_LOGIC;
    IP2Bus_Error                   : out STD_LOGIC
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

--------------------------------------------------------------------------------
-- Architecture section
--------------------------------------------------------------------------------
architecture IMP of user_logic is
--------------------------------------------------------------------------------
-- Component declarations
--------------------------------------------------------------------------------
	component fx2_core is
	port (
		--clk							: in  STD_LOGIC;
		rst							: in  STD_LOGIC;
		-- FX2
		USB_IFCLK            		: in  STD_LOGIC;
		USB_SLRD             		: out STD_LOGIC;
		USB_SLWR             		: out STD_LOGIC;
		USB_FLAGA            		: in  STD_LOGIC;	-- Not used
		USB_FLAGB            		: in  STD_LOGIC;
		USB_FLAGC            		: in  STD_LOGIC;
		USB_FLAGD            		: in  STD_LOGIC;
		USB_SLOE             		: out STD_LOGIC;
		USB_PKTEND           		: out STD_LOGIC;
		USB_FIFOADR         		: out STD_LOGIC_VECTOR( 1 downto 0);
		USB_FD_T             		: out STD_LOGIC_VECTOR( 7 downto 0);
		USB_FD_O             		: out STD_LOGIC_VECTOR( 7 downto 0);
		USB_FD_I             		: in  STD_LOGIC_VECTOR( 7 downto 0);
		-- AXI Stream
		S_AXIS_ACLK					: in  STD_LOGIC;
		S_AXIS_TREADY				: out STD_LOGIC;
		S_AXIS_TDATA				: in  STD_LOGIC_VECTOR(31 downto 0);
		S_AXIS_TKEEP				: in  STD_LOGIC_VECTOR( 3 downto 0);
		S_AXIS_TLAST				: in  STD_LOGIC;
		S_AXIS_TVALID				: in  STD_LOGIC;
		M_AXIS_ACLK					: in  STD_LOGIC;
		M_AXIS_TVALID				: out STD_LOGIC;
		M_AXIS_TDATA				: out STD_LOGIC_VECTOR(31 downto 0);
		M_AXIS_TKEEP				: out STD_LOGIC_VECTOR( 3 downto 0);
		M_AXIS_TLAST				: out STD_LOGIC;
		M_AXIS_TREADY				: in  STD_LOGIC;
		-- Controls
		rx_fifo_reset				: in  STD_LOGIC;
		tx_fifo_reset				: in  STD_LOGIC;
		wr_ep_reg					: in  STD_LOGIC_VECTOR( 1 downto 0);	
		-- ChipScope
		CHIPSCOPE					: out STD_LOGIC_VECTOR(63 downto 0);
		dbgin						: in  STD_LOGIC
	);
	end component fx2_core;
--------------------------------------------------------------------------------
--USER signal declarations added here, as needed for user logic
--------------------------------------------------------------------------------
-- Signals for user logic slave model s/w accessible register example
--------------------------------------------------------------------------------
	signal slv_reg1               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg2               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg3               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg4               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg5               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg6               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg7               : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_reg_write_sel      : STD_LOGIC_VECTOR(7 downto 0);
	signal slv_reg_read_sel       : STD_LOGIC_VECTOR(7 downto 0);
	signal slv_ip2bus_data        : STD_LOGIC_VECTOR(C_SLV_DWIDTH-1 downto 0);
	signal slv_read_ack           : STD_LOGIC;
	signal slv_write_ack          : STD_LOGIC;
  
	signal rx_fifo_reset			: STD_LOGIC;
	signal tx_fifo_reset			: STD_LOGIC;
	signal wr_ep_reg				: STD_LOGIC_VECTOR(1 downto 0);
	signal rst						: STD_LOGIC;
--------------------------------------------------------------------------------
begin
--------------------------------------------------------------------------------

  --USER logic implementation added here

  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(7 downto 0);
  slv_reg_read_sel  <= Bus2IP_RdCE(7 downto 0);
  slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1) or Bus2IP_WrCE(2) or Bus2IP_WrCE(3) or Bus2IP_WrCE(4) or Bus2IP_WrCE(5) or Bus2IP_WrCE(6) or Bus2IP_WrCE(7);
  slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1) or Bus2IP_RdCE(2) or Bus2IP_RdCE(3) or Bus2IP_RdCE(4) or Bus2IP_RdCE(5) or Bus2IP_RdCE(6) or Bus2IP_RdCE(7);

  -- implement slave model software accessible register(s)
	SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
	begin
		if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
			if Bus2IP_Resetn = '0' then
				slv_reg1 <= CONV_STD_LOGIC_VECTOR(C_DEF_WR_EP,slv_reg1'length);
				slv_reg2 <= (others => '0');
				slv_reg3 <= (others => '0');
				slv_reg4 <= (others => '0');
				slv_reg5 <= (others => '0');
				slv_reg6 <= (others => '0');
				slv_reg7 <= (others => '0');
		
				rx_fifo_reset	<= '1';
				tx_fifo_reset	<= '1';
				
			else
				rx_fifo_reset	<= '0';
				tx_fifo_reset	<= '0';
				
				case slv_reg_write_sel is
					when "10000000" =>	-- 0 - RST 
						rx_fifo_reset	<= '1';
						tx_fifo_reset	<= '1';
					when "01000000" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg1(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00100000" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg2(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00010000" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg3(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00001000" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg4(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00000100" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg5(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00000010" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg6(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when "00000001" =>
						for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
							if ( Bus2IP_BE(byte_index) = '1' ) then
								slv_reg7(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
							end if;
						end loop;
					when others => null;
				end case;
			end if;
		end if;
	end process SLAVE_REG_WRITE_PROC;
	
	-- Aviable write EP is EP2 EP4 EP6
	--"00"=EP2,"01"=EP4,"10"=EP6,11"=EP8
	process(Bus2IP_Clk)
	begin
		if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
			case slv_reg1 is
				when x"0000_0002" => wr_ep_reg <= "00";
				when x"0000_0004" => wr_ep_reg <= "01";
				when x"0000_0006" => wr_ep_reg <= "10";
				when others => null;
			end case;
		end if;
	end process;

  -- implement slave model software accessible register(s) read mux
	SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg1, slv_reg2, slv_reg3, slv_reg4, slv_reg5, slv_reg6, slv_reg7 ) is
	begin
		case slv_reg_read_sel is
			when "10000000" => slv_ip2bus_data <= (others => '0');
			when "01000000" => slv_ip2bus_data <= slv_reg1;
			when "00100000" => slv_ip2bus_data <= slv_reg2;
			when "00010000" => slv_ip2bus_data <= slv_reg3;
			when "00001000" => slv_ip2bus_data <= slv_reg4;
			when "00000100" => slv_ip2bus_data <= slv_reg5;
			when "00000010" => slv_ip2bus_data <= slv_reg6;
			when "00000001" => slv_ip2bus_data <= slv_reg7;
			when others => slv_ip2bus_data <= (others => '0');
		end case;
	end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';
  rst	<= not Bus2IP_Resetn;
--------------------------------------------------------------------------------
	fx2_core_inst: fx2_core
	port map(
		rst					=> rst,
		-- FX2
		USB_IFCLK			=> USB_IFCLK,
		USB_SLRD			=> USB_SLRD,
		USB_SLWR			=> USB_SLWR,
		USB_FLAGA			=> USB_FLAGA,
		USB_FLAGB			=> USB_FLAGB,
		USB_FLAGC			=> USB_FLAGC,
		USB_FLAGD			=> USB_FLAGD,
		USB_SLOE			=> USB_SLOE,
		USB_PKTEND			=> USB_PKTEND,
		USB_FIFOADR			=> USB_FIFOADR,
		USB_FD_T			=> USB_FD_T,
		USB_FD_O			=> USB_FD_O,
		USB_FD_I			=> USB_FD_I,
		-- AXI Stream
		S_AXIS_ACLK			=> S_AXIS_ACLK,
		S_AXIS_TREADY		=> S_AXIS_TREADY,
		S_AXIS_TDATA		=> S_AXIS_TDATA,
		S_AXIS_TKEEP		=> S_AXIS_TKEEP,
		S_AXIS_TLAST		=> S_AXIS_TLAST,
		S_AXIS_TVALID		=> S_AXIS_TVALID,
		M_AXIS_ACLK			=> M_AXIS_ACLK,
		M_AXIS_TVALID		=> M_AXIS_TVALID,
		M_AXIS_TDATA		=> M_AXIS_TDATA,
		M_AXIS_TKEEP		=> M_AXIS_TKEEP,
		M_AXIS_TLAST		=> M_AXIS_TLAST,
		M_AXIS_TREADY		=> M_AXIS_TREADY,
		-- Controls
		rx_fifo_reset		=> rx_fifo_reset,
		tx_fifo_reset		=> tx_fifo_reset,
		wr_ep_reg			=> wr_ep_reg,
		-- ChipScope
		CHIPSCOPE			=> CHIPSCOPE,
		dbgin				=> dbgin
	);
--------------------------------------------------------------------------------
end IMP;
