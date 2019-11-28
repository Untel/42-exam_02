/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 16:21:24 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/28 16:23:13 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct	s_flags
{
	int		pad;
	int		prec;
	char	conv;
}				t_flags;

int	ft_atoi(const char *str, int *idx)
{
	int count = 0;

	while (*str && *str >= '0' && *str <= '9')
	{
		count = count * 10 + (*str - '0');
		str++;
		*idx = *idx + 1;
	}
	return (count);
}

int	ft_extract_flags(const char *str, t_flags *f)
{
	int i = 1;

	f->pad = 0;
	f->prec = -1;
	while (str[i])
		if (str[i] >= '0' && str[i] <= '9')
			f->pad = ft_atoi(&str[i], &i);
		else if (str[i] == '.' && i++)
			f->prec = ft_atoi(&str[i], &i);
		else
			break;
	f->conv = str[i];
	return (i);
}

int	ft_itoa_base(char *buff, unsigned int value, char *base, int base_size)
{
	int i = 0;

	buff[4095 - i++] = base[value % base_size];
	while ((value /= base_size) > 0)
		buff[4095 - i++] = base[value % base_size];
	return (i);
}

int	ft_parseint(t_flags f, int nb)
{
	char	buff[4096];
	int		size = 0;
	
	buff[0] = 0;
	size = ft_itoa_base(buff, (unsigned int)(nb < 0 ? -nb : nb), "0123456789", 10);
	if (f.prec > -1)
		while (size < f.prec)
			buff[4095 - size++] = '0';
	if (nb < 0)
		buff[4095 - size++] = '-';
	while (size < f.pad)
		buff[4095 - size++] = ' ';
	write(1, &buff[4095 - size + 1], size);
	return (size);
}

int	ft_parsehex(t_flags f, int nb)
{
	char	buff[4096];
	int		size = 0;
	unsigned int val;

	buff[0] = 0;
	val = (unsigned int)nb;
	size = ft_itoa_base(buff, val, "0123456789abcdef", 16);
	if (f.prec > -1)
		while (size < f.prec)
			buff[4095 - size++] = '0';
	while (size < f.pad)
		buff[4095 - size++] = ' ';
	write(1, &buff[4095 - size + 1], size);
	return (size);
}

int ft_parsestring(t_flags f, char *str)
{
	int len = 0;
	while (str[len])
		len++;
	len = f.prec > -1 && f.prec < len ? f.prec : len;
	while (f.pad-- > len)
		write(1, " ", 1);
	write(1, str, len);
	return (len);
}

int	ft_convert(t_flags f, va_list args)
{
	int len;

	len = 0;
	if (f.conv == 'd')
		len = ft_parseint(f, va_arg(args, int));
	else if (f.conv == 'x')
		len = ft_parsehex(f, va_arg(args, unsigned int));
	else if (f.conv == 's')
		len = ft_parsestring(f, va_arg(args, char *));
	return (len);
}

int	ft_printf(const char *str, ...)
{
	int i = -1;
	int len = 0;
	t_flags f;
	va_list args;

	va_start(args, str);
	len = 0;
	while (str[++i])
		if (str[i] != '%' && ++len)
			write(1, &str[i], 1);
		else
		{
			i += ft_extract_flags(&str[i], &f);
			len += ft_convert(f, args);
		}
	va_end(args);
	return (len);
}

#include <stdio.h>
int a;
int b;
#define PRINT(...)\
{\
	printf("ft_printf(%s)\n", #__VA_ARGS__);\
	a = printf(__VA_ARGS__);\
	printf(" (%d)\n", a);\
	fflush(stdout);\
	b = ft_printf(__VA_ARGS__);\
	printf(" (%d)\n\n", b);\
}
int	main(void)
{
	PRINT("Hello");
	PRINT("Hello %d", 42);
	PRINT("Hello %x yo", -9999);
	PRINT("Hello %s", "you");

	PRINT("Hello %.1d", 42);
	PRINT("Hello %.2x yo", -9999);
	PRINT("Hello %.1s", "you");

	PRINT("Hello %.10d", 42);
	PRINT("Hello %10.5d", -42);
	PRINT("Hello %.15x yo", -9999);

	PRINT("Hello %.15d %s %d yo %x", -9999, "yo", 42, -100);
	PRINT("Hello %.15d %s %d yo %x", -9999, "yo", 42, -100);
}
