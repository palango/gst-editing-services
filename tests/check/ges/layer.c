/* GStreamer Editing Services
 * Copyright (C) 2010 Edward Hervey <bilboed@bilboed.com>
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

#include <ges/ges.h>
#include <gst/check/gstcheck.h>

#define LAYER_HEIGHT 1000

static gboolean
my_fill_track_func (GESTimelineObject * object,
    GESTrackObject * trobject, GstElement * gnlobj, gpointer user_data)
{
  GstElement *src;

  GST_DEBUG ("timelineobj:%p, trackobjec:%p, gnlobj:%p",
      object, trobject, gnlobj);

  /* Let's just put a fakesource in for the time being */
  src = gst_element_factory_make ("fakesrc", NULL);

  /* If this fails... that means that there already was something
   * in it */
  fail_unless (gst_bin_add (GST_BIN (gnlobj), src));

  return TRUE;
}

#define gnl_object_check(gnlobj, start, duration, mstart, mduration, priority, active) { \
  guint64 pstart, pdur, pmstart, pmdur;					\
  guint32 pprio;							\
  gboolean pact;							\
  g_object_get (gnlobj, "start", &pstart, "duration", &pdur,		\
		"media-start", &pmstart, "media-duration", &pmdur,	\
		"priority", &pprio, "active", &pact,			\
		NULL);							\
  assert_equals_uint64 (pstart, start);					\
  assert_equals_uint64 (pdur, duration);					\
  assert_equals_uint64 (pmstart, mstart);					\
  assert_equals_uint64 (pmdur, mduration);					\
  assert_equals_int (pprio, priority);					\
  assert_equals_int (pact, active);					\
  }


GST_START_TEST (test_layer_properties)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  GESTrack *track;
  GESTrackObject *trackobject;
  GESTimelineObject *object;

  ges_init ();

  /* Timeline and 1 Layer */
  timeline = ges_timeline_new ();
  layer = (GESTimelineLayer *) ges_timeline_layer_new ();

  /* The default priority is 0 */
  fail_unless_equals_int (ges_timeline_layer_get_priority (layer), 0);

  /* Layers are initially floating, once we add them to the timeline,
   * the timeline will take that reference. */
  fail_unless (g_object_is_floating (layer));
  fail_unless (ges_timeline_add_layer (timeline, layer));
  fail_if (g_object_is_floating (layer));

  track = ges_track_new (GES_TRACK_TYPE_CUSTOM, GST_CAPS_ANY);
  fail_unless (track != NULL);
  fail_unless (ges_timeline_add_track (timeline, track));

  object =
      (GESTimelineObject *) ges_custom_timeline_source_new (my_fill_track_func,
      NULL);
  fail_unless (object != NULL);

  /* Set some properties */
  g_object_set (object, "start", (guint64) 42, "duration", (guint64) 51,
      "in-point", (guint64) 12, NULL);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_START (object), 42);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_DURATION (object), 51);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_INPOINT (object), 12);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_PRIORITY (object), 0);

  /* Add the object to the timeline */
  fail_unless (g_object_is_floating (object));
  fail_unless (ges_timeline_layer_add_object (layer,
          GES_TIMELINE_OBJECT (object)));
  fail_if (g_object_is_floating (object));
  trackobject = ges_timeline_object_find_track_object (object, track,
      G_TYPE_NONE);
  fail_unless (trackobject != NULL);

  /* This is not a SimpleLayer, therefore the properties shouldn't have changed */
  assert_equals_uint64 (GES_TIMELINE_OBJECT_START (object), 42);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_DURATION (object), 51);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_INPOINT (object), 12);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_PRIORITY (object), 0);
  gnl_object_check (ges_track_object_get_gnlobject (trackobject), 42, 51, 12,
      51, 0, TRUE);

  /* Change the priority of the layer */
  g_object_set (layer, "priority", 1, NULL);
  assert_equals_int (ges_timeline_layer_get_priority (layer), 1);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_PRIORITY (object), 0);
  gnl_object_check (ges_track_object_get_gnlobject (trackobject), 42, 51, 12,
      51, LAYER_HEIGHT, TRUE);

  /* Change it to an insanely high value */
  g_object_set (layer, "priority", 31, NULL);
  assert_equals_int (ges_timeline_layer_get_priority (layer), 31);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_PRIORITY (object), 0);
  gnl_object_check (ges_track_object_get_gnlobject (trackobject), 42, 51, 12,
      51, LAYER_HEIGHT * 31, TRUE);

  /* and back to 0 */
  g_object_set (layer, "priority", 0, NULL);
  assert_equals_int (ges_timeline_layer_get_priority (layer), 0);
  assert_equals_uint64 (GES_TIMELINE_OBJECT_PRIORITY (object), 0);
  gnl_object_check (ges_track_object_get_gnlobject (trackobject), 42, 51, 12,
      51, 0, TRUE);

  g_object_unref (trackobject);
  fail_unless (ges_timeline_layer_remove_object (layer, object));
  fail_unless (ges_timeline_remove_track (timeline, track));
  fail_unless (ges_timeline_remove_layer (timeline, layer));
  g_object_unref (timeline);
}

GST_END_TEST;

GST_START_TEST (test_layer_priorities)
{
  GESTrack *track;
  GESTimeline *timeline;
  GESTimelineLayer *layer1, *layer2, *layer3;
  GESTrackObject *tckobj1, *tckobj2, *tckobj3;
  GESTimelineObject *object1, *object2, *object3;
  GstElement *gnlobj1, *gnlobj2, *gnlobj3;
  guint prio1, prio2, prio3;
  GList *objs, *tmp;

  ges_init ();

  /* Timeline and 3 Layer */
  timeline = ges_timeline_new ();
  layer1 = (GESTimelineLayer *) ges_timeline_layer_new ();
  layer2 = (GESTimelineLayer *) ges_timeline_layer_new ();
  layer3 = (GESTimelineLayer *) ges_timeline_layer_new ();

  ges_timeline_layer_set_priority (layer2, 1);
  ges_timeline_layer_set_priority (layer3, 2);

  fail_unless (ges_timeline_add_layer (timeline, layer1));
  fail_unless (ges_timeline_add_layer (timeline, layer2));
  fail_unless (ges_timeline_add_layer (timeline, layer3));
  fail_unless_equals_int (ges_timeline_layer_get_priority (layer1), 0);
  fail_unless_equals_int (ges_timeline_layer_get_priority (layer2), 1);
  fail_unless_equals_int (ges_timeline_layer_get_priority (layer3), 2);

  track = ges_track_video_raw_new ();
  fail_unless (track != NULL);
  fail_unless (ges_timeline_add_track (timeline, track));

  object1 =
      GES_TIMELINE_OBJECT (ges_custom_timeline_source_new (my_fill_track_func,
          NULL));
  object2 =
      GES_TIMELINE_OBJECT (ges_custom_timeline_source_new (my_fill_track_func,
          NULL));
  object3 =
      GES_TIMELINE_OBJECT (ges_custom_timeline_source_new (my_fill_track_func,
          NULL));
  fail_unless (object1 != NULL);
  fail_unless (object2 != NULL);
  fail_unless (object3 != NULL);

  /* Set priorities on the objects */
  g_object_set (object1, "priority", 0, NULL);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object1), 0);
  g_object_set (object2, "priority", 1, NULL);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object2), 1);
  g_object_set (object3, "priority", LAYER_HEIGHT + 1, NULL);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object3), LAYER_HEIGHT + 1);

  /* Add objects to the timeline */
  fail_unless (ges_timeline_layer_add_object (layer1, object1));
  tckobj1 = ges_timeline_object_find_track_object (object1, track, G_TYPE_NONE);
  fail_unless (tckobj1 != NULL);

  fail_unless (ges_timeline_layer_add_object (layer2, object2));
  tckobj2 = ges_timeline_object_find_track_object (object2, track, G_TYPE_NONE);
  fail_unless (tckobj2 != NULL);

  fail_unless (ges_timeline_layer_add_object (layer3, object3));
  tckobj3 = ges_timeline_object_find_track_object (object3, track, G_TYPE_NONE);
  fail_unless (tckobj3 != NULL);

  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object1), 0);
  gnlobj1 = ges_track_object_get_gnlobject (tckobj1);
  fail_unless (gnlobj1 != NULL);
  g_object_get (gnlobj1, "priority", &prio1, NULL);
  assert_equals_int (prio1, 0);

  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object2), 1);
  gnlobj2 = ges_track_object_get_gnlobject (tckobj2);
  fail_unless (gnlobj2 != NULL);
  g_object_get (gnlobj2, "priority", &prio2, NULL);
  /* object2 is on the second layer and has a priority of 1 */
  assert_equals_int (prio2, LAYER_HEIGHT + 1);

  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object3), LAYER_HEIGHT - 1);
  gnlobj3 = ges_track_object_get_gnlobject (tckobj3);
  fail_unless (gnlobj3 != NULL);
  /* object3 is on the third layer and has a priority of LAYER_HEIGHT + 1
   * it priority must have the maximum priority of this layer*/
  g_object_get (gnlobj3, "priority", &prio3, NULL);
  assert_equals_int (prio3, LAYER_HEIGHT * 3 - 1);

  /* Move layers around */
  g_object_set (layer1, "priority", 2, NULL);
  g_object_set (layer2, "priority", 0, NULL);
  g_object_set (layer3, "priority", 1, NULL);

  /* And check the new priorities */
  assert_equals_int (ges_timeline_layer_get_priority (layer1), 2);
  assert_equals_int (ges_timeline_layer_get_priority (layer2), 0);
  assert_equals_int (ges_timeline_layer_get_priority (layer3), 1);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object1), 0);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object2), 1);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object3), LAYER_HEIGHT - 1);
  g_object_get (gnlobj1, "priority", &prio1, NULL);
  g_object_get (gnlobj2, "priority", &prio2, NULL);
  g_object_get (gnlobj3, "priority", &prio3, NULL);
  assert_equals_int (prio1, 2 * LAYER_HEIGHT);
  assert_equals_int (prio2, 1);
  assert_equals_int (prio3, LAYER_HEIGHT * 2 - 1);

  /* And move objects around */
  fail_unless (ges_timeline_object_move_to_layer (object2, layer1));
  fail_unless (ges_timeline_object_move_to_layer (object3, layer1));

  objs = ges_timeline_layer_get_objects (layer1);
  assert_equals_int (g_list_length (objs), 3);
  fail_unless (ges_timeline_layer_get_objects (layer2) == NULL);
  fail_unless (ges_timeline_layer_get_objects (layer3) == NULL);

  for (tmp = objs; tmp; tmp = g_list_next (tmp)) {
    g_object_unref (tmp->data);
  }
  g_list_free (objs);

  /*  Check their priorities (layer1 priority is now 2) */
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object1), 0);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object2), 1);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object3), LAYER_HEIGHT - 1);
  g_object_get (gnlobj1, "priority", &prio1, NULL);
  g_object_get (gnlobj2, "priority", &prio2, NULL);
  g_object_get (gnlobj3, "priority", &prio3, NULL);
  assert_equals_int (prio1, 2 * LAYER_HEIGHT);
  assert_equals_int (prio2, 2 * LAYER_HEIGHT + 1);
  assert_equals_int (prio3, LAYER_HEIGHT * 3 - 1);

  /* And change TrackObject-s priorities and check that changes are well
   * refected on it containing TimelineObject */
  ges_track_object_set_priority (tckobj3, LAYER_HEIGHT * 2);
  g_object_get (gnlobj3, "priority", &prio3, NULL);
  assert_equals_int (prio3, 2 * LAYER_HEIGHT);
  assert_equals_int (GES_TIMELINE_OBJECT_PRIORITY (object3), 0);

  g_object_unref (tckobj1);
  g_object_unref (tckobj2);
  g_object_unref (tckobj3);
  g_object_unref (timeline);
}

GST_END_TEST;

GST_START_TEST (test_layer_automatic_transition)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  GESTimelineTestSource *src, *srcbis;
  GList *objects = NULL, *tmp = NULL;
  gboolean res = FALSE;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  g_object_set (layer, "auto-transition", TRUE, NULL);
  src = ges_timeline_test_source_new ();
  srcbis = ges_timeline_test_source_new ();

  g_object_set (srcbis, "start", (gint64) 5000, "duration", (gint64) 10000LL,
      NULL);
  g_object_set (src, "start", (gint64) 0, "duration", (gint64) 10000LL, NULL);

  ges_timeline_layer_add_object (layer, GES_TIMELINE_OBJECT (src));
  ges_timeline_layer_add_object (layer, GES_TIMELINE_OBJECT (srcbis));

  objects = ges_timeline_layer_get_objects (layer);

  for (tmp = objects; tmp; tmp = tmp->next) {
    if (GES_IS_TIMELINE_STANDARD_TRANSITION (tmp->data)) {
      res = TRUE;
    }
  }

  fail_unless (res == TRUE);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_string)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gchar *result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_STRING, "", "");

  ges_metadata_container_set_string (GES_METADATA_CONTAINER (layer),
      "ges-test", "blub");

  fail_unless (ges_metadata_container_get_string (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  assert_equals_string (result, "blub");
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_boolean)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gboolean result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_BOOLEAN, "", "");

  ges_metadata_container_set_boolean (GES_METADATA_CONTAINER (layer),
      "ges-test", TRUE);

  fail_unless (ges_metadata_container_get_boolean (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  fail_unless (result);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_int)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gint result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_INT, "", "");

  ges_metadata_container_set_int (GES_METADATA_CONTAINER (layer),
      "ges-test", 1234);

  fail_unless (ges_metadata_container_get_int (GES_METADATA_CONTAINER (layer),
          "ges-test", &result));

  assert_equals_int (result, 1234);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_uint)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  guint result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_UINT, "", "");

  ges_metadata_container_set_uint (GES_METADATA_CONTAINER (layer),
      "ges-test", 42);

  fail_unless (ges_metadata_container_get_uint (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  assert_equals_int (result, 42);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_int64)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gint64 result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_INT64, "", "");

  ges_metadata_container_set_int64 (GES_METADATA_CONTAINER (layer),
      "ges-test", 1234);

  fail_unless (ges_metadata_container_get_int64 (GES_METADATA_CONTAINER (layer),
          "ges-test", &result));

  assert_equals_int64 (result, 1234);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_uint64)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  guint64 result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_UINT64, "", "");

  ges_metadata_container_set_uint64 (GES_METADATA_CONTAINER (layer),
      "ges-test", 42);

  fail_unless (ges_metadata_container_get_uint64 (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  assert_equals_uint64 (result, 42);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_float)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gfloat result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_FLOAT, "", "");

  ges_metadata_container_set_float (GES_METADATA_CONTAINER (layer),
      "ges-test", 23.456);

  fail_unless (ges_metadata_container_get_float (GES_METADATA_CONTAINER (layer),
          "ges-test", &result));

  assert_equals_int64 (result, 23.456);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_double)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  gdouble result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_DOUBLE, "", "");

  ges_metadata_container_set_double (GES_METADATA_CONTAINER (layer),
      "ges-test", 23.456);

  fail_unless (ges_metadata_container_get_double (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  //TODO CHECK
  assert_equals_float (result, 23.456);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_date)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  GDate *input;
  GDate *result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", G_TYPE_DATE, "", "");

  input = g_date_new_dmy (1, 1, 2012);

  ges_metadata_container_set_date (GES_METADATA_CONTAINER (layer),
      "ges-test", input);

  fail_unless (ges_metadata_container_get_date (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  fail_unless (g_date_compare (result, input) == 0);

  g_date_free (input);
  g_date_free (result);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_date_time)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  GstDateTime *input;
  GstDateTime *result;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test", GST_TYPE_DATE_TIME, "", "");

  input = gst_date_time_new_from_unix_epoch_local_time (123456789);

  ges_metadata_container_set_date_time (GES_METADATA_CONTAINER (layer),
      "ges-test", input);

  fail_unless (ges_metadata_container_get_date_time (GES_METADATA_CONTAINER
          (layer), "ges-test", &result));

  //TODO CHECK
  fail_unless (gst_date_time_get_day (input) == gst_date_time_get_day (result));
  fail_unless (gst_date_time_get_hour (input) ==
      gst_date_time_get_hour (result));

  gst_date_time_unref (input);
  gst_date_time_unref (result);
}

GST_END_TEST;

GST_START_TEST (test_layer_metadata_value)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;
  GValue data = G_VALUE_INIT;
  GValue result = G_VALUE_INIT;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("ges-test-value", G_TYPE_VALUE, "", "");

  g_value_init (&data, G_TYPE_STRING);
  g_value_set_string (&data, "Hello world!");

  ges_metadata_container_set_value (GES_METADATA_CONTAINER (layer),
      "ges-test-value", &data);

  fail_unless (ges_metadata_container_get_value (GES_METADATA_CONTAINER
          (layer), "ges-test-value", &result));
  assert_equals_string (g_value_get_string (&result), "Hello world!");
}

GST_END_TEST;

static void
test_foreach (const GESMetadataContainer * container, const gchar * key,
    GValue * value, gpointer user_data)
{
  fail_unless ((0 == g_strcmp0 (key, "some-string")) ||
      (0 == g_strcmp0 (key, "some-int")));
}

GST_START_TEST (test_layer_metadata_foreach)
{
  GESTimeline *timeline;
  GESTimelineLayer *layer;

  ges_init ();

  timeline = ges_timeline_new_audio_video ();
  layer = ges_timeline_layer_new ();
  ges_timeline_add_layer (timeline, layer);

  ges_metadata_register ("some-string", G_TYPE_STRING, "", "");
  ges_metadata_register ("some-int", G_TYPE_INT, "", "");

  ges_metadata_container_set_string (GES_METADATA_CONTAINER (layer),
      "some-string", "some-content");

  ges_metadata_container_set_int (GES_METADATA_CONTAINER (layer),
      "some-int", 123456);

  ges_metadata_container_foreach (GES_METADATA_CONTAINER (layer),
      (GESMetadataForeachFunc) test_foreach, NULL);
}

GST_END_TEST;

static Suite *
ges_suite (void)
{
  Suite *s = suite_create ("ges-timeline-layer");
  TCase *tc_chain = tcase_create ("timeline-layer");

  suite_add_tcase (s, tc_chain);

  tcase_add_test (tc_chain, test_layer_properties);
  tcase_add_test (tc_chain, test_layer_priorities);
  tcase_add_test (tc_chain, test_layer_automatic_transition);
  tcase_add_test (tc_chain, test_layer_metadata_string);
  tcase_add_test (tc_chain, test_layer_metadata_boolean);
  tcase_add_test (tc_chain, test_layer_metadata_int);
  tcase_add_test (tc_chain, test_layer_metadata_uint);
  tcase_add_test (tc_chain, test_layer_metadata_int64);
  tcase_add_test (tc_chain, test_layer_metadata_uint64);
  tcase_add_test (tc_chain, test_layer_metadata_float);
  tcase_add_test (tc_chain, test_layer_metadata_double);
  tcase_add_test (tc_chain, test_layer_metadata_date);
  tcase_add_test (tc_chain, test_layer_metadata_date_time);
  tcase_add_test (tc_chain, test_layer_metadata_value);
  tcase_add_test (tc_chain, test_layer_metadata_foreach);

  return s;
}

int
main (int argc, char **argv)
{
  int nf;

  Suite *s = ges_suite ();
  SRunner *sr = srunner_create (s);

  gst_check_init (&argc, &argv);

  srunner_run_all (sr, CK_NORMAL);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);

  return nf;
}
