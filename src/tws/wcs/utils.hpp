/*
  Copyright (C) 2014 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib GeoWeb Services.

  TerraLib GeoWeb Services is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  TerraLib GeoWeb Services is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */


/*!
  \file tws/wcs/utils.hpp

  \brief Utils for WCS module.

  \author Raphael Willian da Costa
 */

#ifndef __TWS_WCS_UTILS_HPP__
#define __TWS_WCS_UTILS_HPP__

#include <rapidxml/rapidxml.hpp>
namespace tws
{
  namespace wcs
  {
    /*!
      \brief It initializes the WCS XML namespaces
      \param doc A rapidxml::xml_document reference that will be used to create xml nodes
      \return a pointer root node.
    */
    rapidxml::xml_node<char>* initialize_wcs_xml_namespaces(rapidxml::xml_document<char>& doc);

  }   // end namespace wcs
}     // end namespace tws

#endif  // __TWS_WCS_WCS_HPP__
