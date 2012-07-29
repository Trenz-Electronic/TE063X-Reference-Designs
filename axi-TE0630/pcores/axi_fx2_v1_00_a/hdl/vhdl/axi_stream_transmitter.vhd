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
-- Create Date:    19:58 07/24/2012 
-- Design Name:    axi-TE0630
-- Target Devices: all TE USB modules
-- Tool versions:  ISE 13.2
-- Description:    Transmitter for AXI Stream
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
entity axi_stream_transmitter is
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
end entity axi_stream_transmitter;
--------------------------------------------------------------------------------
-- Architecture section
--------------------------------------------------------------------------------
architecture rtl of axi_stream_transmitter is
--------------------------------------------------------------------------------
-- Constant Declarations
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- Signals declaration
--------------------------------------------------------------------------------
	signal word_cnt		: NATURAL range 0 to C_PACKET_LENGTH - 1;
--------------------------------------------------------------------------------
-- Implementation
--------------------------------------------------------------------------------
begin
--------------------------------------------------------------------------------
	M_AXIS_TVALID		<= not rx_fifo_empty;
	M_AXIS_TDATA		<= rx_fifo_rd_data;
	M_AXIS_TKEEP		<= "1111";
	M_AXIS_TLAST		<= '1' when (word_cnt = (C_PACKET_LENGTH - 1)) else '0';
	rx_fifo_rd			<= M_AXIS_TREADY and (not rx_fifo_empty);
	
	process(M_AXIS_ACLK,rst)
	begin
		if(rst = '1')then
			word_cnt	<= 0;
		elsif(M_AXIS_ACLK = '1' and M_AXIS_ACLK'event)then
			if(M_AXIS_TREADY = '1' and rx_fifo_empty = '0')then
				if(word_cnt = (C_PACKET_LENGTH - 1))then
					word_cnt	<= 0;
				else
					word_cnt	<= word_cnt + 1;
				end if;
			end if;
		end if;
	end process;
--------------------------------------------------------------------------------
end rtl;