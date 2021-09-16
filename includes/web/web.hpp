/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 22:40:53 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/16 16:12:46 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_HPP
#define WEB_HPP

#include <netinet/in.h>

namespace web {

/* --- Types ---------------------------------------------------------------- */

typedef int					fd_type;
typedef struct sockaddr_in	addr_type;
typedef socklen_t			addrLen_type;

/* --- Classes -------------------------------------------------------------- */

class 						Socket;
class						Device;
class 						Server;

}; /* namespace web */

#endif /* WEB_HPP */