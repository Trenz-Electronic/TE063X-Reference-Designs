--------------------------------------------------------------------------------
-- Copyright (C) 2012 Trenz Electronic
--
-- Permission is hereby granted, free of charge, to any person obtaining a 
-- copy of this software and associated documentation files (the "Software"), 
-- to deal in the Software without restriction, including without limitation 
-- the rights to use, copy, modify, merge, publish, distribute, sublicense, 
-- and/or sell copies of the Software, and to permit persons to whom the 
-- Software is furnished to do so, subject to the following conditions:
-- 
-- The above copyright notice and this permission notice shall be included 
-- in all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
-- OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
-- FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
-- IN THE SOFTWARE.
--------------------------------------------------------------------------------
-- Company: Trenz Electronics GmbH
-- Engineer: Oleksandr Kiyenko (a.kienko@gmail.com)
-- 
-- Create Date:    22:36:35 05/16/2012 
-- Design Name:    axi-TE0630
-- Target Devices: all TE USB modules
-- Tool versions:  ISE 13.2
-- Description:    Cypress FX2 Slave FIFO interface
--------------------------------------------------------------------------------
-- Libraries
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--------------------------------------------------------------------------------
-- Entity section
--------------------------------------------------------------------------------
entity fx2_core is
port (
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
end entity fx2_core;
--------------------------------------------------------------------------------
-- Architecture section
--------------------------------------------------------------------------------
architecture rtl of fx2_core is
--------------------------------------------------------------------------------
-- Constant Declarations
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- Components Declarations
--------------------------------------------------------------------------------
component fx2_engine is
port (
	rst							: in  STD_LOGIC;
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
	wr_ep						: in  STD_LOGIC_VECTOR( 1 downto 0);
	tx_fifo_rd					: out STD_LOGIC;
	tx_fifo_data				: in  STD_LOGIC_VECTOR( 7 downto 0);
	tx_fifo_empty				: in  STD_LOGIC;
	tx_fifo_valid				: in  STD_LOGIC;
	rx_fifo_wr					: out STD_LOGIC;
	rx_fifo_data				: out STD_LOGIC_VECTOR( 7 downto 0);
	rx_fifo_full				: in  STD_LOGIC;
	rx_fifo_almostfull			: in  STD_LOGIC;
    ChipScope        			: out STD_LOGIC_VECTOR(31 downto 0)
);
end component fx2_engine;

component tx_fifo is
port (
	rst							: in  STD_LOGIC;
    wr_clk 						: in  STD_LOGIC;
    rd_clk 						: in  STD_LOGIC;
    din 						: in  STD_LOGIC_VECTOR(31 downto 0);
    wr_en 						: in  STD_LOGIC;
    rd_en 						: in  STD_LOGIC;
    dout 						: out STD_LOGIC_VECTOR( 7 downto 0);
    full 						: out STD_LOGIC;
    almost_full 				: out STD_LOGIC;
    empty 						: out STD_LOGIC;
    almost_empty 				: out STD_LOGIC;
    valid 						: out STD_LOGIC
);
end component tx_fifo;

component rx_fifo is
port (
    rst 						: in  STD_LOGIC;
    wr_clk 						: in  STD_LOGIC;
    rd_clk 						: in  STD_LOGIC;
    din 						: in  STD_LOGIC_VECTOR( 7 downto 0);
    wr_en 						: in  STD_LOGIC;
    rd_en 						: in  STD_LOGIC;
    dout 						: out STD_LOGIC_VECTOR(31 downto 0);
    full 						: out STD_LOGIC;
    almost_full 				: out STD_LOGIC;
    empty 						: out STD_LOGIC;
    almost_empty 				: out STD_LOGIC
);
end component rx_fifo;

component axi_stream_transmitter is
generic (
	C_PACKET_LENGTH				: INTEGER := 8
);
port (
	rst							: in  STD_LOGIC;
	M_AXIS_ACLK					: in  STD_LOGIC;
	M_AXIS_TVALID				: out STD_LOGIC;
	M_AXIS_TDATA				: out STD_LOGIC_VECTOR(31 downto 0);
	M_AXIS_TKEEP				: out STD_LOGIC_VECTOR( 3 downto 0);
	M_AXIS_TLAST				: out STD_LOGIC;
	M_AXIS_TREADY				: in  STD_LOGIC;
	rx_fifo_rd					: out STD_LOGIC;
	rx_fifo_empty				: in  STD_LOGIC;
	rx_fifo_rd_data				: in  STD_LOGIC_VECTOR(31 downto 0)
);
end component axi_stream_transmitter;

--------------------------------------------------------------------------------
-- Signals declaration
--------------------------------------------------------------------------------
	signal	tx_fifo_rd					: STD_LOGIC;
	signal	tx_fifo_rd_data				: STD_LOGIC_VECTOR( 7 downto 0);
	signal	tx_fifo_empty				: STD_LOGIC;
	signal	tx_fifo_valid				: STD_LOGIC;
	signal	rx_fifo_wr					: STD_LOGIC;
	signal	rx_fifo_wr_data				: STD_LOGIC_VECTOR( 7 downto 0);
	signal	rx_fifo_full				: STD_LOGIC;
	signal	rx_fifo_almost_full			: STD_LOGIC;
    signal	engine_chipscope			: STD_LOGIC_VECTOR(31 downto 0);
    signal	tx_fifo_wr_data				: STD_LOGIC_VECTOR(31 downto 0);
    signal	tx_fifo_wr					: STD_LOGIC;
    signal	tx_fifo_full				: STD_LOGIC;
    signal	tx_fifo_almost_full			: STD_LOGIC;
    signal	rx_fifo_rd					: STD_LOGIC;
    signal	rx_fifo_rd_data				: STD_LOGIC_VECTOR(31 downto 0);
    signal	rx_fifo_empty				: STD_LOGIC;
    signal	rx_fifo_almost_empty		: STD_LOGIC;
	signal	tx_fifo_almost_empty		: STD_LOGIC;
--------------------------------------------------------------------------------
-- Implementation
--------------------------------------------------------------------------------
begin
--------------------------------------------------------------------------------
fx2_engine_inst: fx2_engine
port map(
	rst					=> rst,
    USB_IFCLK           => USB_IFCLK,
    USB_SLRD            => USB_SLRD,
    USB_SLWR            => USB_SLWR,
    USB_FLAGA           => USB_FLAGA,
    USB_FLAGB           => USB_FLAGB,
    USB_FLAGC           => USB_FLAGC,
	USB_FLAGD           => USB_FLAGD,
	USB_SLOE            => USB_SLOE,
	USB_PKTEND          => USB_PKTEND,
	USB_FIFOADR         => USB_FIFOADR,
    USB_FD_T            => USB_FD_T,
    USB_FD_O            => USB_FD_O,
    USB_FD_I            => USB_FD_I,
	wr_ep				=> wr_ep_reg,
	tx_fifo_rd			=> tx_fifo_rd,
	tx_fifo_data		=> tx_fifo_rd_data,
	tx_fifo_empty		=> tx_fifo_empty,
	tx_fifo_valid		=> tx_fifo_valid,
	rx_fifo_wr			=> rx_fifo_wr,
	rx_fifo_data		=> rx_fifo_wr_data,
	rx_fifo_full		=> rx_fifo_full,
	rx_fifo_almostfull	=> rx_fifo_almost_full,
    ChipScope        	=> engine_chipscope
);

tx_fifo_inst: tx_fifo
port map(
	rst					=> rst,
    wr_clk 				=> S_AXIS_ACLK,
    rd_clk 				=> USB_IFCLK,
    din 				=> tx_fifo_wr_data,
    wr_en 				=> tx_fifo_wr,
    rd_en 				=> tx_fifo_rd,
    dout 				=> tx_fifo_rd_data,
    full 				=> tx_fifo_full,
    almost_full 		=> tx_fifo_almost_full,
    empty 				=> tx_fifo_empty,
    almost_empty 		=> tx_fifo_almost_empty,
    valid 				=> tx_fifo_valid
);

rx_fifo_inst: rx_fifo
port map(
    rst 				=> rst,
    wr_clk 				=> USB_IFCLK,
    rd_clk 				=> M_AXIS_ACLK,
    din 				=> rx_fifo_wr_data,
    wr_en 				=> rx_fifo_wr,
    rd_en 				=> rx_fifo_rd,
    dout 				=> rx_fifo_rd_data,
    full 				=> rx_fifo_full,
    almost_full 		=> rx_fifo_almost_full,
    empty 				=> rx_fifo_empty,
    almost_empty 		=> rx_fifo_almost_empty
);

	S_AXIS_TREADY		<= not tx_fifo_almost_full;
	tx_fifo_wr_data		<= S_AXIS_TDATA;
	--S_AXIS_TLAST;
	tx_fifo_wr			<= S_AXIS_TVALID and (not tx_fifo_almost_full);
	
	
axi_stream_tx: axi_stream_transmitter
generic map (
	C_PACKET_LENGTH		=> 8
)
port map (
	rst					=> rst,
	M_AXIS_ACLK			=> M_AXIS_ACLK,
	M_AXIS_TVALID		=> M_AXIS_TVALID,
	M_AXIS_TDATA		=> M_AXIS_TDATA,
	M_AXIS_TKEEP		=> M_AXIS_TKEEP,
	M_AXIS_TLAST		=> M_AXIS_TLAST,
	M_AXIS_TREADY		=> M_AXIS_TREADY,
	rx_fifo_rd			=> rx_fifo_rd,
	rx_fifo_empty		=> rx_fifo_empty,
	rx_fifo_rd_data		=> rx_fifo_rd_data
);
	
	--M_AXIS_TVALID		<= not rx_fifo_empty;
	--M_AXIS_TDATA		<= rx_fifo_rd_data;
	--M_AXIS_TKEEP		<= "1111";
	--M_AXIS_TLAST		<= '0';
	--rx_fifo_rd			<= M_AXIS_TREADY and (not rx_fifo_empty);
	
--	USB_IFCLK &			-- UCB_CLK
--	slrd_drv &			-- SLRD
--	tx_fifo_valid & 	-- SLWR
--	sloe_drv &			-- SLOE
--	pktend_drv &
--	fifoaddr_drv &		-- FIFOADR [2]
--	USB_FD_I &
--	USB_FLAGB &
--	USB_FLAGC &
--	USB_FLAGD &
	
	CHIPSCOPE			<= 
		engine_chipscope(31) & 	-- USB_CLK
		engine_chipscope(30) & 	-- SLRD
		engine_chipscope(29) & 	-- SLWR
		engine_chipscope(28) & 	-- SLOE
		engine_chipscope(27) & 	-- PKTEND
		engine_chipscope(26 downto 25) & -- FIFOADR
		engine_chipscope(24 downto 17) & -- DATA
		engine_chipscope(16) & 	-- FLAGB
		engine_chipscope(15) & 	-- FLAGC
		engine_chipscope(14) & 	-- FLAGD
		tx_fifo_rd_data & 		-- 8
		tx_fifo_rd &			--
		rx_fifo_wr_data &		-- 8
		rx_fifo_wr &			--
		(not tx_fifo_almost_full) &	-- S_AXIS_TREADY
		S_AXIS_TVALID &
		(not rx_fifo_empty) &		-- M_AXIS_TVALID
		M_AXIS_TREADY &
		rx_fifo_full &
		dbgin &
		b"00_0000_0000_0000_0000_0000"
		;

--------------------------------------------------------------------------------
end rtl;
