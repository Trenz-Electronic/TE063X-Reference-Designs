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
-- Create Date: 09/04/2012 
-- Design Name: SLIP test
-- Module Name: ip_engine - RTL 
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
-------------------------------------------------------------------------------
entity ip_engine is
port ( 
    aclk				: in  STD_LOGIC;
    aresetn				: in  STD_LOGIC;
    s_axis_tvalid		: in  STD_LOGIC;
    s_axis_tready		: out STD_LOGIC;
    s_axis_tdata		: in  STD_LOGIC_VECTOR(7 downto 0);
	s_axis_tlast		: in  STD_LOGIC;
    m_axis_tvalid		: out STD_LOGIC;
    m_axis_tready		: in  STD_LOGIC;
	m_axis_tdata		: out STD_LOGIC_VECTOR(7 downto 0);
	m_axis_tlast		: out STD_LOGIC
);
end ip_engine;
-------------------------------------------------------------------------------
architecture RTL of ip_engine is
-------------------------------------------------------------------------------
constant ICMP_REQUEST		: STD_LOGIC_VECTOR( 7 downto 0) := x"08";
constant ICMP_REPLY			: STD_LOGIC_VECTOR( 7 downto 0) := x"00";

type sm_state_type is (
	ST_IDLE,
	ST_ESC_END,
	ST_ESC_ESC,
	ST_END,
	ST_SEND
	);
signal sm_state			: sm_state_type := ST_IDLE;

type ip_addr_type is array (3 downto 0) of STD_LOGIC_VECTOR(7 downto 0);
signal board_ip	: ip_addr_type;
signal host_ip	: ip_addr_type;
-------------------------------------------------------------------------------
begin
-------------------------------------------------------------------------------
-- Pass through
m_axis_tvalid	<= s_axis_tvalid;
s_axis_tready	<= m_axis_tready;
m_axis_tdata	<= s_axis_tdata;
m_axis_tlast 	<= s_axis_tlast;

--process(aclk,aresetn)
--begin
--	if(aresetn = '0')then
--		sm_state			<= ST_IDLE;
--		s_axis_tready_i		<= '1';
--		m_axis_tvalid		<= '0';
--		m_axis_tdata		<= (others => '0');
--	elsif(aclk = '1' and aclk'event)then
--		case sm_state is
--			when ST_IDLE =>
--				s_axis_tready_i	<= '1';	-- We ready to receive
--				if(s_axis_tready_i = '1' and s_axis_tvalid = '1')then
--					s_axis_tready_i		<= '0';	-- Stop read
--					m_axis_tvalid		<= '1';
					--if(s_axis_tdata = ICMP_REQUEST)then
--						m_axis_tdata	<= SLIP_ESC;
--						sm_state		<= ST_ESC_END;
--					elsif(s_axis_tdata = SLIP_ESC)then
--						m_axis_tdata	<= SLIP_ESC;
--						sm_state		<= ST_ESC_ESC;
--					else
--						m_axis_tdata	<= s_axis_tdata;
--						if(s_axis_tlast = '1')then
--							sm_state	<= ST_END;
--						else
--							sm_state	<= ST_SEND;
--						end if;
--					end if;
--				end if;
--			
--			when ST_ESC_END =>
--				if(m_axis_tready = '1')then
--					m_axis_tdata		<= SLIP_ESC_END;
--					sm_state			<= ST_SEND;
--				end if;
--
--			when ST_ESC_ESC =>
--				if(m_axis_tready = '1')then
--					m_axis_tdata		<= SLIP_ESC_ESC;
--					sm_state			<= ST_SEND;
--				end if;
--
--			when ST_END =>
--				if(m_axis_tready = '1')then
--					m_axis_tdata		<= SLIP_END;
--					sm_state			<= ST_SEND;
--				end if;
--				
--			when ST_SEND =>
--				if(m_axis_tready = '1')then
--					m_axis_tvalid		<= '0';
--					sm_state			<= ST_IDLE;
--				end if;
--				
--		end case;
--	end if;
--end process;

-------------------------------------------------------------------------------
end RTL;
