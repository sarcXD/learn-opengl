// Collection of opengl helper functions that can't/should not be abstracted out

/*
 *  Reading a texture using stb_image
 *  Then assigning it to a rectangle vertex with an element buffer
 */
  BufferO BO_Container = CreateRectangleTextured(vertices, v_sz, indices, i_sz);
  BO_Container.TextureO = (unsigned int*)malloc(sizeof(unsigned int)*15); // at max we can have 15 textures on one object
  // load texture using stb image loader
  int width, height, nrChannels;
  unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    Texture2D container = (Texture2D){.width=width,.height=height,.nrChannels=nrChannels,.data=data};

    glBindVertexArray(BO_Container.VAO);

    unsigned int TextureO;
    glGenTextures(1, &TextureO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureO);
    // defining texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // defining texture filtering options with mipmaps
    /* List of texture filtering options with mipmaps"
     * GL_LINEAR_MIPMAP_LINEAR
     * GL_LINEAR_MIPMAP_NEAREST
     * GL_NEAREST_MIPMAP_LINEAR
     * GL_NEAREST_MIPMAP_NEAREST
     *
     * TODO: understand how all the different options work
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, container.width, container.height, 0, GL_RGB, GL_UNSIGNED_BYTE, container.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    BO_Container.TextureO[0] = TextureO;

    glBindVertexArray(0);
    stbi_image_free(container.data);
  }
  else
  {
    // TODO: logging
  }

// ... Some code ...


 glUseProgram(ShaderProgram);
 // here Texture0 is the name of a uniform variable used in a fragment shader
 glUniform1i(glGetUniformLocation(ShaderProgram, "Texture0"), 0);
 // ************************** END SAMPLE ***************************
