----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:56:01 02/14/2012 
-- Design Name: 
-- Module Name:    blinker - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity blinker is
    Port ( sys_clk_pin : in  STD_LOGIC;
           fpga_0_LED_GPIO_d_out_pin : out  STD_LOGIC_VECTOR (3 downto 0)
			  );
end blinker;

architecture Behavioral of blinker is
signal cnt	: STD_LOGIC_VECTOR(25 downto 0) := (others => '0');
begin

process(sys_clk_pin)
begin
	if(sys_clk_pin = '1' and sys_clk_pin'event)then
		cnt <= cnt + 1;
	end if;
end process;

fpga_0_LED_GPIO_d_out_pin	<= cnt(25 downto 22);

end Behavioral;

