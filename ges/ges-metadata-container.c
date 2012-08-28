/* GStreamer Editing Services
 * Copyright (C) 2012 Paul Lange <palango@gmx.de>
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

#include <glib-object.h>
#include <gst/gst.h>

#include "ges-metadata-container.h"

/**
* SECTION: ges-metadata-container
* @short_description: Interface for storing metadata
*
* An interface that allows reading and writing metadata
*/

static GQuark ges_taglist_key;
static GstTagMergeMode merge_mode;

G_DEFINE_INTERFACE_WITH_CODE (GESMetadataContainer, ges_metadata_container,
    G_TYPE_OBJECT, ges_taglist_key =
    g_quark_from_static_string ("ges-metadata-container-data");
    merge_mode = GST_TAG_MERGE_KEEP;);

static void
ges_metadata_container_default_init (GESMetadataContainerInterface * iface)
{

}

static GstTagList *
ges_metadata_container_get_taglist (GESMetadataContainer * container)
{
  GstTagList *list;

  list = g_object_get_qdata (G_OBJECT (container), ges_taglist_key);
  if (!list) {
    list = gst_tag_list_new_empty ();
    g_object_set_qdata (G_OBJECT (container), ges_taglist_key, list);
  }

  return list;
}

typedef struct
{
  GESMetadataForeachFunc func;
  const GESMetadataContainer *container;
  gpointer data;
} MetadataForeachData;

static void
tag_list_foreach (const GstTagList * taglist, const gchar * tag,
    gpointer user_data)
{
  MetadataForeachData *data = (MetadataForeachData *) user_data;
  data->func (data->container, tag, data->data);
}

/**
 * ges_metadata_container_foreach:
 * @container: container to iterate over
 * @func: (scope call): function to be called for each tag
 * @user_data: (closure): user specified data
 *
 * Calls the given function for each tag inside the metadata container. Note
 * that if there is no tag, the function won't be called at all.
 */
void
ges_metadata_container_foreach (GESMetadataContainer * container,
    GESMetadataForeachFunc func, gpointer user_data)
{
  GstTagList *list;
  MetadataForeachData foreach_data;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (func != NULL);

  list = ges_metadata_container_get_taglist (container);

  foreach_data.func = func;
  foreach_data.container = container;
  foreach_data.data = user_data;

  gst_tag_list_foreach (list, (GstTagForeachFunc) tag_list_foreach,
      &foreach_data);
}

/**
 * ges_metadata_container_set_boolean:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_boolean (GESMetadataContainer * container,
    const gchar * metadata_item, gboolean value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_int:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_int (GESMetadataContainer * container,
    const gchar * metadata_item, gint value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_uint:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_uint (GESMetadataContainer * container,
    const gchar * metadata_item, guint value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_int64:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_int64 (GESMetadataContainer * container,
    const gchar * metadata_item, gint64 value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_uint64:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_uint64 (GESMetadataContainer * container,
    const gchar * metadata_item, guint64 value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_float:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_float (GESMetadataContainer * container,
    const gchar * metadata_item, gfloat value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_double:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_double (GESMetadataContainer * container,
    const gchar * metadata_item, gdouble value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_date:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_date (GESMetadataContainer * container,
    const gchar * metadata_item, const GDate * value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_date_time:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_date_time (GESMetadataContainer * container,
    const gchar * metadata_item, const GstDateTime * value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_string:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_string (GESMetadataContainer * container,
    const gchar * metadata_item, const gchar * value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add (list, merge_mode, metadata_item, value, NULL);
}

/**
 * ges_metadata_container_set_value:
 * @container: Target container
 * @metadata_item: Name of the metadata item to set
 * @value: Value to set
 * Sets the value of a given metadata item
 */
void
ges_metadata_container_set_value (GESMetadataContainer * container,
    const gchar * metadata_item, const GValue * value)
{
  GstTagList *list;

  g_return_if_fail (GES_IS_METADATA_CONTAINER (container));
  g_return_if_fail (metadata_item != NULL);

  list = ges_metadata_container_get_taglist (container);

  gst_tag_list_add_value (list, merge_mode, metadata_item, value);
}

/**
 * ges_metadata_container_to_string:
 * @container: a #GESMetadataContainer
 *
 * Serializes a metadata container to a string.
 *
 * Returns: a newly-allocated string, or NULL in case of an error. The
 *    string must be freed with g_free() when no longer needed.
 */
gchar *
ges_metadata_container_to_string (GESMetadataContainer * container)
{
  GstTagList *list;

  g_return_val_if_fail (GES_IS_METADATA_CONTAINER (container), NULL);

  list = ges_metadata_container_get_taglist (container);

  return gst_tag_list_to_string (list);
}

/**
 * ges_metadata_container_new_from_string:
 * @str: a string created with ges_metadata_container_to_string()
 *
 * Deserializes a metadata container.
 *
 * Returns: (transfer full): a new #GESMetadataContainer, or NULL in case of an
 * error.
 */
GESMetadataContainer *
ges_metadata_container_new_from_string (const gchar * str)
{
  return NULL;
}

/**
 * ges_metadata_register:
 * @name: the name or identifier string (string constant)
 * @type: the type this data is in
 * @nick: human-readable name or short description (string constant)
 * @blurb: a human-readable description for this tag (string constant)
 *
 * Registers a new tag type for the use with GES.
 *
 * See <link linkend="gstreamer-GstTagList">GstTagList</link> for a list of
 * already available tags.
 */
void
ges_metadata_register (const gchar * name, GType type, const gchar * nick,
    const gchar * blurb)
{
  g_return_if_fail (name != NULL);
  g_return_if_fail (nick != NULL);
  g_return_if_fail (blurb != NULL);
  g_return_if_fail (type != 0 && type != GST_TYPE_LIST);

  gst_tag_register (name, GST_TAG_FLAG_META, type, nick, blurb, NULL);
}

/* Copied from gsttaglist.c */
/***** evil macros to get all the *_get_* functions right *****/

#define CREATE_GETTER(name,type)                                         \
gboolean                                                                 \
ges_metadata_container_get_ ## name (GESMetadataContainer *container,    \
                           const gchar *metadata_item, type value)       \
{                                                                        \
  GstTagList *list;                                                      \
                                                                         \
  g_return_val_if_fail (GES_IS_METADATA_CONTAINER (container), FALSE);   \
  g_return_val_if_fail (metadata_item != NULL, FALSE);                   \
  g_return_val_if_fail (value != NULL, FALSE);                           \
                                                                         \
  list = ges_metadata_container_get_taglist (container);                 \
                                                                         \
  return gst_tag_list_get_ ## name (list, metadata_item, value);         \
}

/**
 * ges_metadata_container_get_boolean:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (boolean, gboolean *);
/**
 * ges_metadata_container_get_int:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (int, gint *);
/**
 * ges_metadata_container_get_uint:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (uint, guint *);
/**
 * ges_metadata_container_get_int64:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (int64, gint64 *);
/**
 * ges_metadata_container_get_uint64:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (uint64, guint64 *);
/**
 * ges_metadata_container_get_float:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (float, gfloat *);
/**
 * ges_metadata_container_get_double:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (double, gdouble *);

/**
 * ges_metadata_container_get_string:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (string, gchar **);

/**
 * ges_metadata_container_get_value:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
gboolean
ges_metadata_container_get_value (GESMetadataContainer * container,
    const gchar * tag, GValue * value)
{
  GstTagList *list;

  g_return_val_if_fail (GES_IS_METADATA_CONTAINER (container), FALSE);
  g_return_val_if_fail (tag != NULL, FALSE);
  g_return_val_if_fail (value != NULL, FALSE);

  list = ges_metadata_container_get_taglist (container);

  if (!gst_tag_list_copy_value (value, list, tag))
    return FALSE;

  return (value != NULL);
}

/**
 * ges_metadata_container_get_date:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (date, GDate **);

/**
 * ges_metadata_container_get_date_time:
 * @container: Target container
 * @metadata_item: Name of the metadata item to get
 * @dest: Destination to which value of metadata item will be copied
 * Gets the value of a given metadata item, returns NULL if @metadata_item
 * can not be found.
 */
CREATE_GETTER (date_time, GstDateTime **);
