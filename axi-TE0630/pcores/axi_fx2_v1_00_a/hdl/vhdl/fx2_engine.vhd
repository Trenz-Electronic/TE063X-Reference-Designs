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
entity fx2_engine is
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
end entity fx2_engine;
--------------------------------------------------------------------------------
-- Architecture section
--------------------------------------------------------------------------------
architecture rtl of fx2_engine is
--------------------------------------------------------------------------------
-- Constant Declarations
--------------------------------------------------------------------------------
constant C_USB_SLOE_ENABLE_LEVEL	: STD_LOGIC := '1';
constant C_USB_SLOE_DISABLE_LEVEL	: STD_LOGIC := '0';
constant C_USB_SLRD_ACTIVE			: STD_LOGIC := '1';
constant C_USB_SLRD_PASSIVE			: STD_LOGIC := '0';
constant C_USB_SLWR_ACTIVE			: STD_LOGIC := '1';
constant C_USB_SLWR_PASSIVE			: STD_LOGIC := '0';
--------------------------------------------------------------------------------
-- Signals declaration
--------------------------------------------------------------------------------
-- Registers for all input signals
signal usb_tx_empty   	: STD_LOGIC;  -- FX2 transmit fifo empty
signal usb_tx_full    	: STD_LOGIC;  -- FX2 transmit fifo full
signal usb_rx_empty   	: STD_LOGIC;  -- FX2 receive fifo empty
signal fd_i_reg			: STD_LOGIC_VECTOR(7 downto 0);

-- Drivers for outputs
signal  fd_o_drv		: STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
signal  fd_t_drv		: STD_LOGIC_VECTOR(7 downto 0) := (others => '1');
signal	slrd_drv		: STD_LOGIC;
signal	slwr_drv		: STD_LOGIC;
signal	sloe_drv		: STD_LOGIC;
signal	pktend_drv		: STD_LOGIC;
signal	fifoaddr_drv	: STD_LOGIC_VECTOR(1 downto 0);

-- FSM
type wr_state_type is (
	STATE_WR_INIT,
	STATE_WR,
	STATE_WR_REM,
	STATE_READ_1,
	STATE_READ_2
);
signal	wr_fsm_state		: wr_state_type;
	
signal tx_fifo_read_req		: STD_LOGIC;
signal read_fsm_enable		: STD_LOGIC;
signal fd_t_bit				: STD_LOGIC := '1';
signal last_write			: STD_LOGIC;
--------------------------------------------------------------------------------
-- Implementation
--------------------------------------------------------------------------------
begin
--------------------------------------------------------------------------------
-- USB Inputs
--------------------------------------------------------------------------------
process(USB_IFCLK,rst)
begin
	if(rst = '1')then
		usb_tx_empty	<= '1';
	elsif(USB_IFCLK = '1' and USB_IFCLK'event)then
		usb_tx_empty	<= USB_FLAGC;
	end if;
end process;
usb_rx_empty	<= USB_FLAGD;
usb_tx_full		<= USB_FLAGB;

process(USB_IFCLK)
begin
	if(USB_IFCLK = '1' and USB_IFCLK'event)then
		fd_i_reg		<= USB_FD_I;
	end if;
end process;
--------------------------------------------------------------------------------
-- FSM
--------------------------------------------------------------------------------
	process(USB_IFCLK,rst)
	begin
		if(rst = '1')then
			wr_fsm_state			<= STATE_WR_INIT;
			tx_fifo_read_req		<= '0';
			read_fsm_enable			<= '0';	
			rx_fifo_wr				<= '0';
			rx_fifo_data			<= (others => '0');
			last_write 				<= '0';
		elsif(USB_IFCLK = '1' and USB_IFCLK'event)then
			case wr_fsm_state is
				when STATE_WR_INIT =>
					rx_fifo_wr				<= '0';	-- stop write
					sloe_drv				<= C_USB_SLOE_DISABLE_LEVEL;
					slwr_drv				<= C_USB_SLWR_PASSIVE;
					last_write 				<= '0';
					if(usb_rx_empty = '0' and rx_fifo_almostfull = '0')then
					--if(usb_rx_empty = '0')then
						sloe_drv			<= C_USB_SLOE_ENABLE_LEVEL;
						read_fsm_enable		<= '1';
						wr_fsm_state		<= STATE_READ_1;	-- V7
					elsif(tx_fifo_empty = '0' and usb_tx_full = '0')then
						sloe_drv			<= C_USB_SLOE_DISABLE_LEVEL;
						tx_fifo_read_req	<= '1';
						read_fsm_enable		<= '0';
						wr_fsm_state		<= STATE_WR;
					end if;
					
				when STATE_WR =>
					sloe_drv				<= C_USB_SLOE_DISABLE_LEVEL;
					if(tx_fifo_empty = '1')then
						tx_fifo_read_req	<= '0';
						slwr_drv			<= C_USB_SLWR_PASSIVE;
						last_write 			<= '1';
						wr_fsm_state		<= STATE_WR_INIT;
					elsif(usb_tx_full = '1' or usb_rx_empty = '0')then
						tx_fifo_read_req	<= '0';
						slwr_drv			<= C_USB_SLWR_PASSIVE;
						wr_fsm_state		<= STATE_WR_REM;
					else
						tx_fifo_read_req	<= '1';
						slwr_drv			<= C_USB_SLWR_ACTIVE;
					end if;
				
				when STATE_WR_REM =>
					sloe_drv				<= C_USB_SLOE_DISABLE_LEVEL;
					slwr_drv				<= C_USB_SLWR_PASSIVE;
					if(usb_rx_empty = '0' and rx_fifo_almostfull = '0')then
						wr_fsm_state		<= STATE_WR_INIT;
					elsif(usb_tx_full = '0')then
						wr_fsm_state		<= STATE_WR;
					end if;

				when STATE_READ_1 =>
					rx_fifo_wr				<= '0';	-- stop write
					wr_fsm_state			<= STATE_READ_2;
					
				when STATE_READ_2 =>
					rx_fifo_data			<= fd_i_reg;
					rx_fifo_wr				<= '1';	-- Write result
					if(usb_rx_empty = '0' -- We have something to read
						and rx_fifo_almostfull = '0'	-- and have room in FIFO
						)then	-- Go to next read cycle
						sloe_drv			<= C_USB_SLOE_ENABLE_LEVEL;
						wr_fsm_state		<= STATE_READ_1;
					else
						sloe_drv			<= C_USB_SLOE_DISABLE_LEVEL;
						wr_fsm_state		<= STATE_WR_INIT;
					end if;
					
				when others => null;
			end case;
		end if;
	end process;

process(USB_IFCLK)
begin
	if(USB_IFCLK = '1' and USB_IFCLK'event)then
		pktend_drv			<= last_write;
	end if;
end process;

fd_t_bit	<= '0' when 
	(wr_fsm_state = STATE_WR) or 
	(wr_fsm_state = STATE_WR_REM) else '1';

process(USB_IFCLK)
begin
	if USB_IFCLK = '1' and USB_IFCLK'event then
		for i in fd_t_drv'range loop
			fd_t_drv(i) <= fd_t_bit;
		end loop;
	end if;
end process;

process(USB_IFCLK)
begin
	if(USB_IFCLK = '1' and USB_IFCLK'event)then
		if(
			(wr_fsm_state = STATE_WR_INIT or wr_fsm_state = STATE_READ_2) and 
			(usb_rx_empty = '0') and 
			(rx_fifo_almostfull = '0'))then
			slrd_drv			<= C_USB_SLRD_ACTIVE;	-- Start Read
		else
			slrd_drv			<= C_USB_SLRD_PASSIVE;
		end if;
	end if;
end process;
--------------------------------------------------------------------------------
-- FIFOs
--------------------------------------------------------------------------------
fd_o_drv			<= tx_fifo_data;
tx_fifo_rd			<= '1' when (
	(tx_fifo_empty = '0') and 
	(usb_tx_full = '0') and 
	(tx_fifo_read_req = '1')) else '0';
--"00"=EP2,"01"=EP4,"10"=EP6,11"=EP8
fifoaddr_drv		<= "11" when (read_fsm_enable = '1') else wr_ep;
--------------------------------------------------------------------------------
-- USB Outputs
--------------------------------------------------------------------------------
USB_SLRD			<= slrd_drv;
USB_SLWR 			<= tx_fifo_valid;
USB_SLOE			<= sloe_drv;
USB_PKTEND			<= pktend_drv;
USB_FIFOADR			<= fifoaddr_drv;
USB_FD_T			<= fd_t_drv;
USB_FD_O			<= fd_o_drv;
--------------------------------------------------------------------------------
-- Debug
--------------------------------------------------------------------------------
ChipScope 			<= 
	USB_IFCLK &			-- UCB_CLK
	slrd_drv &			-- SLRD
	tx_fifo_valid & 	-- SLWR
	sloe_drv &			-- SLOE
	pktend_drv &
	fifoaddr_drv &		-- FIFOADR [2]
	USB_FD_I &
	USB_FLAGB &
	USB_FLAGC &
	USB_FLAGD &
	b"00_0000_0000_0000"
	;
--------------------------------------------------------------------------------
end rtl;
