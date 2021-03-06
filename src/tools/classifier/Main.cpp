/**
 * Copyright (C) 2013 Universität Osnabrück
 * This file is part of the LAS VEGAS Reconstruction Toolkit,
 *
 * LAS VEGAS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LAS VEGAS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

/**
 * @file Main.cpp
 * @author Simon Herkenhoff <sherkenh@uni-osnabrueck.de>
*/

// Program options for this tool

#ifndef DEBUG
#include "Options.hpp"
#endif

// Local includes
#include <lvr/reconstruction/AdaptiveKSearchSurface.hpp>
#include <lvr/reconstruction/FastReconstruction.hpp>
#include <lvr/io/PLYIO.hpp>
#include <lvr/geometry/Matrix4.hpp>
#include <lvr/geometry/HalfEdgeMesh.hpp>
#include <lvr/texture/Texture.hpp>
#include <lvr/texture/Transform.hpp>
#include <lvr/texture/Texturizer.hpp>
#include <lvr/texture/Statistics.hpp>
#include <lvr/geometry/QuadricVertexCosts.hpp>
#include <lvr/reconstruction/SharpBox.hpp>
#include <lvr/config/lvropenmp.hpp>

#include <iostream>


using namespace lvr;

typedef ColorVertex<float, unsigned char>               cVertex;
typedef Normal<float>                                   cNormal;
typedef AdaptiveKSearchSurface<ColorVertex<float, unsigned char>, Normal<float> >        akSurface;

/**
 * @brief   Main entry point for the LSSR surface executable
 */
int main(int argc, char** argv)
{
	try
	{
		// Parse command line arguments
		classifier::Options options(argc, argv);

		// Exit if options had to generate a usage message
		// (this means required parameters are missing)
		if ( options.printUsage() )
		{
			return 0;
		}

		OpenMPConfig::setNumThreads(options.getNumThreads());

		::std::cout << options << ::std::endl;

		// Create a point loader object
		ModelPtr model = ModelFactory::readModel( options.getInputFileName() );
		PointBufferPtr p_loader;

		// Parse loaded data
		if ( !model )
		{
			cout << timestamp << "IO Error: Unable to parse " << options.getInputFileName() << endl;
			exit(-1);
		}
		p_loader = model->m_pointCloud;

	}
	catch(...)
	{
		std::cout << "Unable to parse options. Call 'classifier --help' for more information." << std::endl;
	}
	return 0;
}

