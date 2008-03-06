#include "TextSet.hpp"

#include "glgui.hpp"
#ifdef HAVE_FREETYPE
#include "Viewpoint.hpp"
#include <R.h>
#include <map>
#endif
//////////////////////////////////////////////////////////////////////////////
//
// CLASS
//   TextSet
//
// INTERNAL TEXTS STORAGE
//   texts are copied to a buffer without null byte
//   a separate length buffer holds string lengths in order
//

TextSet::TextSet(Material& in_material, int in_ntexts, char** in_texts, double *in_center, double in_adj,
                 int in_ignoreExtent, FontArray& in_fonts)
 : Shape(in_material, in_ignoreExtent), textArray(in_ntexts, in_texts)
{
  int i;

  material.lit = false;
  material.colorPerVertex(false);

  adj = in_adj;

  // init vertex array

  vertexArray.alloc(in_ntexts);

  for (i=0;i<in_ntexts;i++) {

    vertexArray[i].x = (float) in_center[i*3+0];
    vertexArray[i].y = (float) in_center[i*3+1];
    vertexArray[i].z = (float) in_center[i*3+2];

    boundingBox += vertexArray[i];
  }
  fonts = in_fonts;
}

TextSet::~TextSet()
{
}

void TextSet::draw(RenderContext* renderContext) {

  int cnt;
  GLFont* font;

  material.beginUse(renderContext);

  StringArrayIterator iter(&textArray);

  for( cnt = 0, iter.first(); !iter.isDone(); iter.next(), cnt++ ) {
    if (!vertexArray[cnt].missing()) {
      material.useColor(cnt);
      glRasterPos3f( vertexArray[cnt].x, vertexArray[cnt].y, vertexArray[cnt].z );
      String text = iter.getCurrent();
      font = fonts[cnt % fonts.size()];
      if (font) 
        font->draw( text.text, text.length, adj, renderContext->gl2psActive );
    }
  }

  material.endUse(renderContext);
}


