/*
 * Copyright (C) 2012-2013 Nicolas Bouillot (http://www.nicolasbouillot.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 */

/** \addtogroup libshmdata-osg
 * provides video reading to an OpenSceneGraph Texture.
 * compile with `pkg-config --cflags --libs shmdata-osg-0.4`
 *  @{
 */

#ifndef _SHM_DATA_OSG_READER_H_
#define _SHM_DATA_OSG_READER_H_

#include <osg/Texture2D>

/**
 * @file   osg-reader.h
 * 
 * @brief Reading video data from a shared memory into an OpenSceneGraph texture 
 * 
 * 
 */

/**
 * \class shmdata::OsgReader
 *
 * \brief Reading video data from a shared memory into an OpenSceneGraph texture.  
 *  Alpha channel is supported.
 */


namespace shmdata
{
  class OsgReader_impl;		// forward declaration

  class OsgReader
  {
  public:
    OsgReader ();

    /** 
     * update the texture with incoming video
     * 
     * @return true started, false otherwise (due to an incorrect or not set socket path).
     */
    void play ();

    /** 
     * do not update the texture with incoming video
     * 
     */
    void pause ();

    /** 
     * Set the socket path and start (and play if pause() has not been called before). 
     * 
     * @return true if the path has been set, false if the socket path is not correct.
     */
    bool setPath (const std::string & socketPath);


    /** 
     * Get video width of the last frame received from the shared memory. 
     * Initialized with -1. 
     * 
     * @return the width
     */
    int getWidth ();

    /** 
     * Get video height of the last frame received from the shared memory. 
     * Initialized with -1. 
     * 
     * @return the height
     */
    int getHeight ();

    /** 
     * Get the texture where video is written. Can be called after constructor.
     * 
     * @return the pointer to the osg::Texture2D instance where video is written
     */
    osg::Texture2D * getTexture ();

    /** 
     * Updates the texture with the latest image from the shared memory
     */
     void updateImage();

     ~OsgReader ();

     /** 
      * Set debug enable printing debug messages
      * 
      * @param debug is the boolean enabling or disabling debug 
      */
    void setDebug (bool debug);
  private:
      OsgReader_impl * impl_;	// PIMPL opaque pointer
  };

}				//end namespace

#endif				//_SHM_DATA_READER_H_

/** @}*/
