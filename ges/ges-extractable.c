/* GStreamer Editing Services
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <gst/pbutils/pbutils.h>
#include "ges.h"
#include "ges-types.h"
#include "ges-material.h"
#include "ges-extractable.h"

G_DEFINE_INTERFACE (GESExtractable, ges_extractable, G_TYPE_OBJECT);
G_DEFINE_TYPE (GESExtractableObject, ges_extractable_object, G_TYPE_OBJECT);

GESMaterial *
ges_extractable_get_material (GESExtractableInterface * self)
{
  return NULL;
}

GType
ges_extractable_get_material_type (GESExtractableInterface * self)
{
  return G_TYPE_OBJECT;
}

static void
ges_extractable_default_init (GESExtractableInterface * self)
{

}


/* Default implementation of extractable */

GParamSpec **
ges_extractable_object_class_get_mandatory_parameters (GESExtractableObjectClass
    * klass, guint * n_pspecs_p)
{
  g_return_val_if_fail (GES_IS_EXTRACTABLE_OBJECT_CLASS (klass), NULL);
  g_return_val_if_fail (klass->mandatory_parameters, NULL);

  return g_param_spec_pool_list (klass->mandatory_parameters, G_TYPE_OBJECT,
      n_pspecs_p);
}

static void
ges_extractable_object_class_init (GESExtractableObjectClass * klass)
{
}

static void
ges_extractable_object_init (GESExtractableObject * self)
{
}
