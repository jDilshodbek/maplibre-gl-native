# Using a GeoJSON Source

This guide will teach you how to use [`GeoJsonSource`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.sources/-geo-json-source/index.html") by deep diving into [`GeoJSON`]("https://geojson.org/") file format.
You will start with fundamentals of how a map renders data internally and why you should prefer [JSON]("https://en.wikipedia.org/wiki/JSON") format in storing geospatial data.

# Goals

After finishing  this documentation you should be able to:
1. Understand how [`Style`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-style/index.html?query=open%20class%20Style"), [`Layer`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.layers/-layer/index.html?query=abstract%20class%20Layer"), and [`Data source`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.sources/-source/index.html?query=abstract%20class%20Source") interact with each other.
2. Explore building blocks of `GeoJSON` data.
3. Use `GeoJSON` files in constructing [`GeoJsonSource`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.sources/-source/index.html?query=abstract%20class%20Source")s.
4. Update data at runtime.

### 1. Styles, Layers, and Data source

- `Style ` defines the visual representation of the map such as colors and appearance.
- `Layer` controls how data should be presented to the user.
- `Data source`  holds actual data and provides layers with it.

Styles consist of collections of layers and data source. Layers reference data source. Hence they require a unique source ID when you construct them.
It would be meaningless if we don't have any data to show, so we need know how to supply data through a data source.
Firstly, we need to understand how to store data and pass it into a data source; therefore, we will discuss JSON in the next session.

### 2. GeoJSON

[`GeoJSON`]("https://geojson.org/") is a JSON file for encoding various geographical data structures.
It defines several JSON objects to represent geospatial information. We use the`.geojson` extension for GeoJSON files.
We define the most fundamental objects:
- `Geometry` refers to a single geometric shape that contains one or more coordinates. These shapes are visual objects displayed on a map. A geometry can be one of the following six types:
    - Point
    - MultiPoint
    - LineString
    - MultilineString
    - Polygon
    - MultiPolygon
- `Feautue` is a compound object that combines a single geometry with user-defined attributes, such as name, color.
- `FeatureCollection` is set of features stored in an array. It is a root object that introduces all other features.

A typical GeoJSON structure might look like:
```kotlin
 {{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/MapSnapshotterWithinExpression.kt:buffer_line}}
```
So far we learned describing Geospatial data in GeoJSON files. We will start applying this knowledge into our map applications.

### 3. GeoJsonSource

As we discussed before, map requires some sort data to be rendered. We use different sources such as [`Vector`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.sources/-vector-source/index.html?query=class%20VectorSource%20:%20Source"), [`Raster`]("https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.style.sources/-raster-source/index.html?query=class%20RasterSource%20:%20Source") and `GeoJSON`.
We will focus exclusively on `GeoJsonSource` and will not address other sources.
`GeoJsonSource` is a type of source that has a unique `String` ID and GeoJSON data.

There are several ways to construct a `GeoJsonSource`:
- Locally stored files such as assets and raw folders
- Remote services
- Raw string  parsed into FeatureCollections objects
- Geometry, Feature, and FeatureCollection objects that map to GeoJSON Base builders

A sample GeoJsonSource:
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CollectionUpdateOnStyleChange.kt:geojson}}
```

Note that you can not simply show data on a map. Layers must reference them.
Therefore, you create a layer that gives visual appearance to it.

- Loading from local files

with assets folder file
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/NoStyleActivity.kt:setup}}
```
with raw folder file
```kotlin
  {{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RuntimeStyleActivity.kt:source}}
```
parsing method:
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/ResourceUtils.kt:read_raw}}
```
- Loading from remote services
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HeatmapLayerActivity.kt:create_earthquake}}
```
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HeatmapLayerActivity.kt:constants}}
```
- Parsing string with `fromJson` method of FeatureCollection
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/MapSnapshotterWithinExpression.kt:buffer_line}}
```
- Creating Geometry, Feature, and FeatureCollections from scratch

Note that the GeoJSON objects we discussed earlier have classes defined in the MapLibre SDK.
Therefore, we can either map JSON objects to regular Java/Kotlin objects or build them directly.
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QuerySourceFeaturesActivity.kt:json_object}}
```

### 4. Updating data at runtime

The key feature of GeoJsonSources is that once we add one, we can set another set of data.
We achieve this using `setGeoJson()` method. For instance, we create a source variable and check if we have not assigned it, then we create a new source object and add it to style; otherwise, we set a different data source:
```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ZoomFunctionSymbolLayerActivity.kt:createFeatureCollection}}
```

```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ZoomFunctionSymbolLayerActivity.kt:updateSource}}
```


Another advanced example showcases random cars and a passenger on a map updating their positions with smooth animation.
The example defines two sources for the cars and passenger objects and we assign these sources to appropriate layers.
We use `ValueAnimator` objects and update positions with `onAnimationUpdate()`:

```kotlin
{{#include ../../../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/AnimatedSymbolLayerActivity.kt}}
```

# Summary

GeoJsonSources have their pros and cons. They are most effective when you want to add additional data to your style or provide features like animating objects on your map.
However, working with large datasets can be challenging if you need to manipulate and store data within the app; in such cases, it’s better to use a remote data source.
Lastly, you can refer to [`Official Maplibre Android Documentation`]("https://maplibre.org/maplibre-native/android/api/index.html") for a comprehensive guide and  [`Test App`]("https://github.com/maplibre/maplibre-native/tree/main/platform/android/MapLibreAndroidTestApp") to learn best practises for your applications.