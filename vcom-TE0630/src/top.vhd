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
-- Create Date:    22:36:35 05/16/2012 
-- Design Name:    vcom-TE0320
-- Module Name:    top - Behavioral 
-- Project Name:   vcom
-- Target Devices: all TE USB modules
-- Tool versions:  ISE 13.2
-- Description:    Simple stub to communicate with FX2 in Virtual COM mode
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-------------------------------------------------------------------------------
entity top is
port ( 
	sys_clk				: in    STD_LOGIC;
	sys_rst				: in    STD_LOGIC;	-- aclive high
	LED					: out   STD_LOGIC_VECTOR(3 downto 0);
	USB_FD_pin			: inout STD_LOGIC_VECTOR(7 downto 0);
	USB_FLAGA_pin		: in    STD_LOGIC;	-- FlagA = EP2EF
	USB_FLAGB_pin		: in    STD_LOGIC;	-- FlagB = EP8FF
	USB_FLAGC_pin		: in    STD_LOGIC;	-- FlagC = EP8PF (half of buffer)
	USB_FLAGD_pin		: in    STD_LOGIC;	-- FlagD = LED (toggle each rx byte)
	USB_SLWR_pin		: out   STD_LOGIC;
	USB_SLRD_pin		: out   STD_LOGIC;
	USB_SLOE_pin		: out   STD_LOGIC;
	USB_PKTEND_pin		: out   STD_LOGIC;
	USB_FIFOADR_pin		: out   STD_LOGIC_VECTOR(1 downto 0);
	USB_IFCLK_pin		: in    STD_LOGIC
);
end top;
-------------------------------------------------------------------------------
architecture Behavioral of top is
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
type sm_state_type is (ST_IDLE, ST_READ, ST_READ_END, ST_WRITE);
signal sm_state			: sm_state_type;
signal rd_data			: STD_LOGIC_VECTOR( 7 downto 0);
-------------------------------------------------------------------------------
begin
-------------------------------------------------------------------------------
usb_clk			<= USB_IFCLK_pin;
usb_empty		<= USB_FLAGA_pin;
usb_full		<= USB_FLAGB_pin;
LED				<= USB_FLAGD_pin & USB_FLAGC_pin & USB_FLAGB_pin & USB_FLAGA_pin;
USB_FD_pin		<= "ZZZZZZZZ" when fd_t_drv = '1' else fd_d_drv;
USB_SLWR_pin	<= slwr;
USB_SLRD_pin	<= slrd;
USB_SLOE_pin	<= sloe;
USB_PKTEND_pin	<= pktend;
USB_FIFOADR_pin	<= fifoadr;

process(usb_clk,sys_rst)
begin
	if(sys_rst = '1')then
		fd_t_drv	<= '1';
	elsif(usb_clk = '1' and usb_clk'event)then
		case sm_state is
			when ST_IDLE => 
				fd_t_drv		<= '1';				
				slwr			<= DISABLE;
				pktend			<= DISABLE;		
				if(usb_empty = DISABLE)then		-- We have data to read
					fifoadr		<= EP2ADR;		-- Read from EP2
					sloe		<= ENABLE;
					sm_state	<= ST_READ;
				end if;
			
			when ST_READ =>
				rd_data			<= USB_FD_pin;	-- Store received byte
				slrd			<= ENABLE;		-- Pop data from FIFO
				sm_state		<= ST_READ_END;
			
			when ST_READ_END =>					-- Return all to initial state
				slrd			<= DISABLE;
				sloe			<= DISABLE;
				sm_state		<= ST_WRITE;
				
			when ST_WRITE =>
				if(usb_full = DISABLE)then		-- If FX2 FIFO not full
					fifoadr		<= EP8ADR;		-- Write to EP8
					fd_t_drv	<= '0';			-- Drive FD bus
					fd_d_drv	<= rd_data + 1;	-- Data to transmit
												-- In this example we just add 1
												-- to received byte 
					slwr		<= ENABLE;		-- Write
					pktend		<= ENABLE;		-- Commit packet
					sm_state	<= ST_IDLE;		
				end if;
		end case;
	end if;
end process;
-------------------------------------------------------------------------------
end Behavioral;
-------------------------------------------------------------------------------
