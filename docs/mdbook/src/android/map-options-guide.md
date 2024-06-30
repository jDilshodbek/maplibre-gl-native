# MapLibreMapOptions

This guide will explain what is [MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html) and various ways to build `MapView` by using [MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html)

Sometimes, you might need a map which is preconfigured and given certain values such that it makes easy to work and focus on only your task.
For instance some apps must show static location of their businesses on the map or they want to use their own service endpoint in the whole application.
As it requires some work, Fortunately there is an easy option. You can setup your map with initial values in one place. It can be implemented both in XML and programmatically.

Below you will see how to build MapView with initial configuration values in XML and programmatically.

1. To give default values of Map , use specific tags starting with `maplibre`.

Specifying `CameraPosition` and some other attributions to show specific place in XML
```xml
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    tools:context=".activity.maplayout.SimpleMapActivity">

    <org.maplibre.android.maps.MapView
        android:id="@id/mapView"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        app:maplibre_cameraTargetLat="41.300648"
        app:maplibre_cameraTargetLng="69.24325"
        app:maplibre_cameraZoom="2"
        app:maplibre_cameraTilt="25"
        app:maplibre_uiLogoGravity = "bottom|end"
        app:maplibre_uiLogoMarginRight = "24dp"
        app:maplibre_uiAttributionGravity = "bottom|end" />

</RelativeLayout>
```

You will have the target point with some customization on the map

![xml result](https://github.com/maplibre/maplibre-native/assets/19223457/e4216647-29d7-41c8-b377-acafca20de64)


2. You can also achieve the same result programmatically with [MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html)

[MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html) is  a helper class which is used to configure MapView programmatically. You can give default zoom level, style and more.

The following code presents creating [MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html) object and assigns it to `MapView` constructor
```Kotlin
private lateinit var mapView: MapView

override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    
    val options = MapLibreMapOptions.createFromAttributes(this, null)
    options.camera(
        CameraPosition.Builder()
            .target(LatLng(52.35273, 4.91638))
            .tilt(25.0)
            .zoom(13.0)
            .build())
    options.styleUrl("asset://streets.json")
    
    mapView = MapView(this,options)
    mapView.id = R.id.mapView
    mapView.onCreate(savedInstanceState)
    mapView.getMapAsync(this)
    setContentView(mapView)
}
```
3. [MapLibreMapOptions](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html) can also be used as an argument for [MapFragment](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-support-map-fragment/index.html?query=open%20class%20SupportMapFragment%20:%20Fragment,%20OnMapReadyCallback) static factory method

The following code snippet shows how to create [MapFragment](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-support-map-fragment/index.html?query=open%20class%20SupportMapFragment%20:%20Fragment,%20OnMapReadyCallback) with static factory method:
```Kotlin
private lateinit var mapView: MapView

override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_map_fragment)
    
    val mapFragment: SupportMapFragment?
    
    if (savedInstanceState == null) {
        val options = MapLibreMapOptions.createFromAttributes(this, null)
        options.scrollGesturesEnabled(false)
        options.zoomGesturesEnabled(false)
        options.tiltGesturesEnabled(false)
        options.rotateGesturesEnabled(false)
        options.debugActive(false)
        val dc = LatLng(38.90252, -77.02291)
        options.minZoomPreference(9.0)
        options.maxZoomPreference(11.0)
        options.camera(
            CameraPosition.Builder()
                .target(dc)
                .zoom(11.0)
                .build()
        )
        mapFragment = SupportMapFragment.newInstance(options)
        supportFragmentManager
            .beginTransaction()
            .add(R.id.fragment_container, mapFragment, TAG)
            .commit()
    } else {
        mapFragment = supportFragmentManager.findFragmentByTag(TAG) as SupportMapFragment?
    }
    mapFragment!!.getMapAsync(this)
}
```
You can find further information about `MapLibreMapOption` in the [documentation](https://maplibre.org/maplibre-native/android/api/-map-libre%20-native%20-android/org.maplibre.android.maps/-map-libre-map-options/index.html)
