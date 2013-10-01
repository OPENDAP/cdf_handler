// CDFGrid.cc

// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of cdf_handler, a C++ server of OPeNDAP for access to cdf
// data

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301
 
// (c) COPYRIGHT University Corporation for Atmostpheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>
 
#include "config_cdf.h"

#include <cdf.h>

#include "CDFGrid.h"

BaseType *
CDFGrid::ptr_duplicate()
{
    return new CDFGrid(*this);
}

CDFGrid::CDFGrid(const string &n, const string &d) : Grid(n, d)
{
}

CDFGrid::CDFGrid(const CDFGrid &rhs) : Grid(rhs)
{
}

CDFGrid::~CDFGrid()
{
}

CDFGrid &
CDFGrid::operator=(const CDFGrid &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<CDFGrid&>(*this) = rhs;


    return *this;
}

