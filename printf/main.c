#include <stdio.h>
#include "ft_printf.h"

int	main(void)
{
	int		ret1, ret2;
	char	c = 'A';
	char	*str = "Hello, 42!";
	int		num = -123;
	unsigned int u = 123456789;
	void	*ptr = str;

	ret1 = ft_printf("ft_printf: char: %c\n", c);
	ret2 = printf("printf   : char: %c\n\n", c);

	ret1 = ft_printf("ft_printf: string: %s\n", str);
	ret2 = printf("printf   : string: %s\n\n", str);

	ret1 = ft_printf("ft_printf: pointer: %p\n", ptr);
	ret2 = printf("printf   : pointer: %p\n\n", ptr);

	ret1 = ft_printf("ft_printf: int: %d %i\n", num, num);
	ret2 = printf("printf   : int: %d %i\n\n", num, num);

	ret1 = ft_printf("ft_printf: unsigned: %u\n", u);
	ret2 = printf("printf   : unsigned: %u\n\n", u);

	ret1 = ft_printf("ft_printf: hex (lower): %x\n", u);
	ret2 = printf("printf   : hex (lower): %x\n\n", u);

	ret1 = ft_printf("ft_printf: hex (upper): %X\n", u);
	ret2 = printf("printf   : hex (upper): %X\n\n", u);

	ret1 = ft_printf("ft_printf: percent: %%\n");
	ret2 = printf("printf   : percent: %%\n\n");

	ft_printf("%c, %s, %p, %d, %i, %u, %x, %X, %%\n", 
		'A', "Hello", "Hello", -42, -42, 123456789u, 255, 255);
 

	return (0);
}
