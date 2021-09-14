/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 10:18:09 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/13 11:20:02 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Device.hpp"

namespace web {
    
void web::Device::setPort(uint32_t port)
{
    _port = port;
}

}; /* namespace web */