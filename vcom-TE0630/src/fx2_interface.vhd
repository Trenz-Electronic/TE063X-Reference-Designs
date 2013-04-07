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
-- Design Name: SLIP test
-- Module Name: fx2_interface - RTL 
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.STD_LOGIC_arith.all;
use ieee.STD_LOGIC_unsigned.all;
-------------------------------------------------------------------------------
entity fx2_interface is
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
end fx2_interface;
-------------------------------------------------------------------------------
architecture RTL of fx2_interface is
-------------------------------------------------------------------------------
-- Endpoints addresses for FIFOADR pins
constant EP2ADR			: STD_LOGIC_VECTOR( 1 downto 0) := "00";
constant EP4ADR			: STD_LOGIC_VECTOR( 1 downto 0) := "01";
constant EP6ADR			: STD_LOGIC_VECTOR( 1 downto 0) := "10";
constant EP8ADR			: STD_LOGIC_VECTOR( 1 downto 0) := "11";
-- Polarity is configured by FIFOPINPOLAR register in FX2 firmware
constant ENABLE			: STD_LOGIC := '1';
constant DISABLE		: STD_LOGIC := '0';
-- Signals
signal fd_d_drv			: STD_LOGIC_VECTOR( 7 downto 0);
signal fd_t_drv			: STD_LOGIC := '1';
signal sloe				: STD_LOGIC := DISABLE;
signal slwr				: STD_LOGIC := DISABLE;
signal slrd				: STD_LOGIC := DISABLE;
signal pktend			: STD_LOGIC := DISABLE;
signal fifoadr			: STD_LOGIC_VECTOR( 1 downto 0);
signal usb_clk			: STD_LOGIC;
signal usb_empty		: STD_LOGIC;
signal usb_full			: STD_LOGIC;
signal usb_read_blok	: STD_LOGIC;
signal usb_write_blok	: STD_LOGIC;
type sm_state_type is (
	ST_IDLE, 
	ST_READ_1, ST_READ_2, ST_READ_3, ST_READ_4, ST_READ_5, ST_READ_END, 
	ST_WRITE_1, ST_WRITE_2, ST_WRITE_3, ST_WRITE_4, ST_WRITE_5,
	ST_WRITE_6, ST_WRITE_7,	ST_WRITE_8
	);
signal sm_state			: sm_state_type := ST_IDLE;
signal byte_cnt			: STD_LOGIC_VECTOR(3 downto 0);
signal last				: STD_LOGIC;
-------------------------------------------------------------------------------
begin
-------------------------------------------------------------------------------
usb_clk			<= USB_IFCLK_pin;
usb_empty		<= USB_FLAGA_pin;
usb_full		<= USB_FLAGB_pin;
usb_read_blok	<= USB_FLAGD_pin;
usb_write_blok	<= USB_FLAGC_pin;
USB_FD_T		<= fd_t_drv;
USB_FD_O		<= fd_d_drv;
USB_SLWR_pin	<= slwr;
USB_SLRD_pin	<= slrd;
USB_SLOE_pin	<= sloe;
USB_PKTEND_pin	<= pktend;
USB_FIFOADR_pin	<= fifoadr;
m_aclk			<= USB_IFCLK_pin;
s_aclk			<= USB_IFCLK_pin;
b_cnt			<= byte_cnt;

process(usb_clk,s_aresetn)
begin
	if(s_aresetn = '0')then
		m_axis_tdata	<= (others => '0');
		m_axis_tvalid	<= '0';
		s_axis_tready	<= '0';
		sm_state		<= ST_IDLE;
	elsif(usb_clk = '1' and usb_clk'event)then
		case sm_state is
			when ST_IDLE => 
				fd_t_drv		<= '1';				
				slrd			<= DISABLE;
				slwr			<= DISABLE;
				pktend			<= DISABLE;		
				sloe			<= DISABLE;
				s_axis_tready	<= '0';
				m_axis_tvalid	<= '0';
				if(usb_empty = DISABLE)then		-- We have data to read
					sm_state		<= ST_READ_1;	-- Start of async read
					fifoadr			<= EP2ADR;		-- Read from EP2
					fd_t_drv		<= '1';			-- Read FD bus
					sloe			<= ENABLE;
				elsif(s_axis_tvalid = '1' and usb_full = DISABLE)then
					sm_state		<= ST_WRITE_1;
					fd_d_drv		<= s_axis_tdata;-- Data to transmit
					last			<= s_axis_tlast;
					s_axis_tready	<= '1';			-- Pop data
					fifoadr			<= EP6ADR;		-- Write to EP6
					fd_t_drv		<= '0';			-- Drive FD bus
				end if;
			
			when ST_READ_1 =>
				sm_state		<= ST_READ_2;
				
			when ST_READ_2 =>
				slrd			<= ENABLE;		-- Pop data from FIFO
				sm_state		<= ST_READ_3;

			when ST_READ_3 =>
				sm_state		<= ST_READ_4;
				
			when ST_READ_4 =>
				sm_state		<= ST_READ_5;
				
			when ST_READ_5 =>
				m_axis_tdata	<= USB_FD_I;	-- Store received byte
				slrd			<= DISABLE;
				sloe			<= DISABLE;
				m_axis_tvalid	<= '1';
				sm_state		<= ST_READ_END;
			
			when ST_READ_END =>					-- Return all to initial state
				if(m_axis_tready = '1')then
					sm_state		<= ST_IDLE;
					m_axis_tvalid	<= '0';
					byte_cnt		<= byte_cnt + 1;
				end if;
				
			when ST_WRITE_1 =>
				slwr			<= ENABLE;		-- Write
				if(last = '1')then
					pktend			<= ENABLE;		-- Commit packet
				else
					pktend			<= DISABLE;		-- Not commit packet
				end if;
				s_axis_tready	<= '0';
				sm_state		<= ST_WRITE_2;
				
			when ST_WRITE_2 =>
				sm_state		<= ST_WRITE_3;

			when ST_WRITE_3 =>
				sm_state		<= ST_WRITE_4;

			when ST_WRITE_4 =>
				pktend			<= DISABLE;
				slwr			<= DISABLE;
				sm_state		<= ST_WRITE_5;
			
			when ST_WRITE_5 =>
				sm_state		<= ST_WRITE_6;

			when ST_WRITE_6 =>
				sm_state		<= ST_WRITE_7;

			when ST_WRITE_7 =>
				sm_state		<= ST_WRITE_8;
				
			when ST_WRITE_8 =>
				sm_state		<= ST_IDLE;
		end case;
	end if;
end process;
-------------------------------------------------------------------------------
end RTL;
