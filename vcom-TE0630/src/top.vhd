-------------------------------------------------------------------------------
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
-------------------------------------------------------------------------------
-- Company: Trenz Electronics GmbH
-- Engineer: Oleksandr Kiyenko (a.kienko@gmail.com)
-- 
-- Create Date: 19:51:49 09/03/2012 
-- Design Name: user test
-- Module Name: top - RTL 
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;
-------------------------------------------------------------------------------
entity top is
port ( 
	sys_clk				: in    STD_LOGIC;
	sys_rst				: in    STD_LOGIC;	-- aclive high
	LED					: out   STD_LOGIC_VECTOR(3 downto 0);
	USB_FD_pin			: inout STD_LOGIC_VECTOR(7 downto 0);
	USB_FLAGA_pin		: in    STD_LOGIC;
	USB_FLAGB_pin		: in    STD_LOGIC;
	USB_FLAGC_pin		: in    STD_LOGIC;
	USB_FLAGD_pin		: in    STD_LOGIC;
	USB_SLWR_pin		: out   STD_LOGIC;
	USB_SLRD_pin		: out   STD_LOGIC;
	USB_SLOE_pin		: out   STD_LOGIC;
	USB_PKTEND_pin		: out   STD_LOGIC;
	USB_FIFOADR_pin		: out   STD_LOGIC_VECTOR(1 downto 0);
	USB_IFCLK_pin		: in    STD_LOGIC
);
end top;
-------------------------------------------------------------------------------
architecture RTL of top is
-------------------------------------------------------------------------------
component ic_fifo is
port (
    m_aclk				: in  STD_LOGIC;
    s_aclk				: in  STD_LOGIC;
    s_aresetn			: in  STD_LOGIC;
    s_axis_tvalid		: in  STD_LOGIC;
    s_axis_tlast		: in  STD_LOGIC;
    s_axis_tready		: out STD_LOGIC;
    s_axis_tdata		: in  STD_LOGIC_VECTOR(7 downto 0);
    m_axis_tvalid		: out STD_LOGIC;
    m_axis_tlast		: out STD_LOGIC;
    m_axis_tready		: in  STD_LOGIC;
	m_axis_tdata		: out STD_LOGIC_VECTOR(7 downto 0)
);
end component ic_fifo;

component fx2_interface is
port ( 
	USB_FD_I			: in  STD_LOGIC_VECTOR(7 downto 0);
	USB_FD_O			: out STD_LOGIC_VECTOR(7 downto 0);
	USB_FD_T			: out STD_LOGIC;
	USB_FLAGA_pin		: in  STD_LOGIC;
	USB_FLAGB_pin		: in  STD_LOGIC;
	USB_FLAGC_pin		: in  STD_LOGIC;
	USB_FLAGD_pin		: in  STD_LOGIC;
	USB_SLWR_pin		: out STD_LOGIC;
	USB_SLRD_pin		: out STD_LOGIC;
	USB_SLOE_pin		: out STD_LOGIC;
	USB_PKTEND_pin		: out STD_LOGIC;
	USB_FIFOADR_pin		: out STD_LOGIC_VECTOR(1 downto 0);
	USB_IFCLK_pin		: in  STD_LOGIC;
    m_aclk				: out STD_LOGIC;
    s_aclk				: out STD_LOGIC;
    s_aresetn			: in  STD_LOGIC;
    s_axis_tvalid		: in  STD_LOGIC;
    s_axis_tlast		: in  STD_LOGIC;
    s_axis_tready		: out STD_LOGIC;
    s_axis_tdata		: in  STD_LOGIC_VECTOR(7 downto 0);
    m_axis_tvalid		: out STD_LOGIC;
    m_axis_tready		: in  STD_LOGIC;
	m_axis_tdata		: out STD_LOGIC_VECTOR(7 downto 0);
	b_cnt				: out STD_LOGIC_VECTOR(3 downto 0)
);
end component fx2_interface;

component user_engine is
port ( 
    aclk				: in  STD_LOGIC;
    aresetn				: in  STD_LOGIC;
    s_axis_tvalid		: in  STD_LOGIC;
    s_axis_tready		: out STD_LOGIC;
    s_axis_tdata		: in  STD_LOGIC_VECTOR(7 downto 0);
    m_axis_tvalid		: out STD_LOGIC;
    m_axis_tlast		: out STD_LOGIC;
    m_axis_tready		: in  STD_LOGIC;
	m_axis_tdata		: out STD_LOGIC_VECTOR(7 downto 0);
	b_cnt				: out STD_LOGIC_VECTOR(3 downto 0)
);
end component user_engine;

-- Signals
signal USB_FD_O				: STD_LOGIC_VECTOR( 7 downto 0);
signal USB_FD_T				: STD_LOGIC;
signal usb_clk				: STD_LOGIC;
signal s_aresetn			: STD_LOGIC;
signal fx2_s_axis_tvalid	: STD_LOGIC;
signal fx2_s_axis_tlast		: STD_LOGIC;
signal fx2_s_axis_tready	: STD_LOGIC;
signal fx2_s_axis_tdata		: STD_LOGIC_VECTOR( 7 downto 0);
signal fx2_m_axis_tvalid	: STD_LOGIC;
signal fx2_m_axis_tready	: STD_LOGIC;
signal fx2_m_axis_tdata		: STD_LOGIC_VECTOR( 7 downto 0);
signal user_s_axis_tvalid	: STD_LOGIC;
signal user_s_axis_tlast	: STD_LOGIC;
signal user_s_axis_tready	: STD_LOGIC;
signal user_s_axis_tdata	: STD_LOGIC_VECTOR( 7 downto 0);
signal user_m_axis_tvalid	: STD_LOGIC;
signal user_m_axis_tlast	: STD_LOGIC;
signal user_m_axis_tready	: STD_LOGIC;
signal user_m_axis_tdata	: STD_LOGIC_VECTOR( 7 downto 0);
signal led_cnt				: STD_LOGIC_VECTOR(26 downto 0);
-------------------------------------------------------------------------------
begin
-------------------------------------------------------------------------------
-- Glue logic
USB_FD_pin		<= "ZZZZZZZZ" when USB_FD_T = '1' else USB_FD_O;
s_aresetn		<= '1';
-- Indication
process(sys_clk,s_aresetn)
begin
	if(s_aresetn='0')then
		led_cnt	<= (others => '0');
	elsif(sys_clk = '1' and sys_clk'event)then
		led_cnt	<= led_cnt + 1;
	end if;
end process;
--LED	<= 
--	(led_cnt(23) and led_cnt(24) and led_cnt(25) and led_cnt(26)) &		-- 1
--	(led_cnt(23) and led_cnt(25) and led_cnt(26)) &						-- 2
--	(led_cnt(23) and (led_cnt(24) or led_cnt(25)) and led_cnt(26)) &	-- 3
--	(led_cnt(23) and led_cnt(26));	

usb_clk		<= 	USB_IFCLK_pin;

fx2_intf: fx2_interface
port map( 
	USB_FD_I			=> USB_FD_pin,
	USB_FD_O			=> USB_FD_O,
	USB_FD_T			=> USB_FD_T,
	USB_FLAGA_pin		=> USB_FLAGA_pin,
	USB_FLAGB_pin		=> USB_FLAGB_pin,
	USB_FLAGC_pin		=> USB_FLAGC_pin,
	USB_FLAGD_pin		=> USB_FLAGD_pin,
	USB_SLWR_pin		=> USB_SLWR_pin,
	USB_SLRD_pin		=> USB_SLRD_pin,
	USB_SLOE_pin		=> USB_SLOE_pin,
	USB_PKTEND_pin		=> USB_PKTEND_pin,
	USB_FIFOADR_pin		=> USB_FIFOADR_pin,
	USB_IFCLK_pin		=> USB_IFCLK_pin,
    m_aclk				=> open,
    s_aclk				=> open,
    s_aresetn			=> s_aresetn,
    s_axis_tvalid		=> fx2_s_axis_tvalid,
    s_axis_tlast		=> fx2_s_axis_tlast,
    s_axis_tready		=> fx2_s_axis_tready,
    s_axis_tdata		=> fx2_s_axis_tdata,
    m_axis_tvalid		=> fx2_m_axis_tvalid,
    m_axis_tready		=> fx2_m_axis_tready,
	m_axis_tdata		=> fx2_m_axis_tdata,
	--b_cnt				=> LED
	b_cnt				=> open
);

fx2user_fifo: ic_fifo
port map(
    m_aclk				=> usb_clk,
    s_aclk				=> usb_clk,	-- can be changed
    s_aresetn			=> s_aresetn,
    s_axis_tvalid		=> fx2_m_axis_tvalid,
    s_axis_tlast		=> '0',
    s_axis_tready		=> fx2_m_axis_tready,
    s_axis_tdata		=> fx2_m_axis_tdata,
    m_axis_tvalid		=> user_s_axis_tvalid,
    m_axis_tlast		=> open,
    m_axis_tready		=> user_s_axis_tready,
	m_axis_tdata		=> user_s_axis_tdata
);

user2fx_fifo: ic_fifo
port map(
    m_aclk				=> usb_clk,	-- can be changed
    s_aclk				=> usb_clk,
    s_aresetn			=> s_aresetn,
    s_axis_tvalid		=> user_m_axis_tvalid,
    s_axis_tlast		=> user_m_axis_tlast,
    s_axis_tready		=> user_m_axis_tready,
    s_axis_tdata		=> user_m_axis_tdata,
    m_axis_tvalid		=> fx2_s_axis_tvalid,
    m_axis_tlast		=> fx2_s_axis_tlast,
    m_axis_tready		=> fx2_s_axis_tready,
	m_axis_tdata		=> fx2_s_axis_tdata
);

user_engine_inst: user_engine
port map( 
    aclk				=> usb_clk,	-- can be changed
    aresetn				=> s_aresetn,
    s_axis_tvalid		=> user_s_axis_tvalid,
    s_axis_tready		=> user_s_axis_tready,
    s_axis_tdata		=> user_s_axis_tdata,
    m_axis_tvalid		=> user_m_axis_tvalid,
    m_axis_tlast		=> user_m_axis_tlast,
    m_axis_tready		=> user_m_axis_tready,
	m_axis_tdata		=> user_m_axis_tdata,
	b_cnt				=> LED

);
-------------------------------------------------------------------------------
end RTL;
