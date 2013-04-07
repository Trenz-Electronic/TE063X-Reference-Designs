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
-- Module Name: user_engine - RTL 
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-------------------------------------------------------------------------------
entity user_engine is
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
end user_engine;
-------------------------------------------------------------------------------
architecture RTL of user_engine is
-------------------------------------------------------------------------------
signal byte_cnt		: STD_LOGIC_VECTOR(3 downto 0);
type sm_state_type is (
	ST_IDLE,
	ST_P1, ST_P2, ST_P3, ST_P4
	);
signal sm_state		: sm_state_type := ST_IDLE;
signal rec_char		: STD_LOGIC_VECTOR( 7 downto 0);
-------------------------------------------------------------------------------
begin
-------------------------------------------------------------------------------
-- loopback
--m_axis_tvalid	<= s_axis_tvalid;
--s_axis_tready	<= m_axis_tready;
--m_axis_tdata	<= s_axis_tdata + 2;
--process(aclk)
--begin
--	if(aclk = '1' and aclk'event)then
--		if(s_axis_tvalid = '1' and m_axis_tready = '1')then
--			byte_cnt	<= byte_cnt + 1;
--		end if;
--	end if;
--end process;
b_cnt	<= rec_char(3 downto 0);

process(aclk)
begin
	if(aclk = '1' and aclk'event)then
		case sm_state is
			when ST_IDLE => 
				s_axis_tready	<= '1';
				m_axis_tlast	<= '0';
				m_axis_tvalid	<= '0';
				rec_char		<= s_axis_tdata;
				if(s_axis_tvalid = '1')then
					sm_state		<= ST_P1;
					s_axis_tready	<= '0';
				end if;
			when ST_P1 =>
				m_axis_tdata	<= x"5b";	-- "["
				m_axis_tvalid	<= '1';
				sm_state		<= ST_P2;
			when ST_P2 =>
				if(m_axis_tready = '1')then
					m_axis_tdata	<= rec_char;
					m_axis_tvalid	<= '1';
					sm_state		<= ST_P3;
				end if;
			when ST_P3 =>
				if(m_axis_tready = '1')then
					m_axis_tdata	<= x"5d";	-- "]"
					m_axis_tvalid	<= '1';
					m_axis_tlast	<= '1';
					sm_state		<= ST_P4;
				end if;
			when ST_P4 =>
				if(m_axis_tready = '1')then
					m_axis_tvalid	<= '0';
					m_axis_tlast	<= '0';
					sm_state		<= ST_IDLE;
				end if;
		end case;
	end if;
end process;
-------------------------------------------------------------------------------
end RTL;
