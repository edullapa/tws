# Web Time Series Service

**Web Time Series Service**, or **WTSS** for short, is a lightweight web service for handling time series data from remote sensing imagery.

```
http://www.dpi.inpe.br/wtss/list_coverages
```

```
{
  "coverages": [
    "fire_risk_monthly",
    "mod09q1",
    "mod13q1"
  ]
}
```
```
http://www.dpi.inpe.br/wtss/describe_coverage?name=mod13q1
```
```
{

    "name": "mod13q1",
    "description": "Vegetation Indices 16-Day L3 Global 250m",
    "detail": "https://lpdaac.usgs.gov/products/modis_products_table/mod13q1",
    "dimensions": 

[

{

    "name": "col_id",
    "description": "column",
    "min_idx": ​0,
    "max_idx": ​0

},
{

    "name": "row_id",
    "description": "row",
    "min_idx": ​0,
    "max_idx": ​0

},

    {
        "name": "time_id",
        "description": "time",
        "min_idx": ​0,
        "max_idx": ​0
    }

],
"attributes": 
[

{

    "name": "ndvi",
    "description": "250m 16 days NDVI",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​-2000,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-3000

},
{

    "name": "evi",
    "description": "250m 16 days EVI",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​-2000,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-3000

},
{

    "name": "quality",
    "description": "250m 16 days VI Quality detailed QA",
    "datatype": "16-bit unsigned integer",
    "valid_range": 

    {
        "min": ​0,
        "max": ​65534
    },
    "scale_factor": ​1,
    "missing_value": ​65535

},
{

    "name": "red",
    "description": "250m 16 days red reflectance (Band 1)",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​0,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-1000

},
{

    "name": "nir",
    "description": "250m 16 days NIR reflectance (Band 2)",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​0,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-1000

},
{

    "name": "blue",
    "description": "250m 16 days blue reflectance (Band 3)",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​0,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-1000

},
{

    "name": "mir",
    "description": "250m 16 days MIR reflectance (Band 7)",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​0,
        "max": ​10000
    },
    "scale_factor": ​0.0001,
    "missing_value": ​-1000

},
{

    "name": "view_zenith",
    "description": "250m 16 days view zenith angle",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​-9000,
        "max": ​9000
    },
    "scale_factor": ​0.01,
    "missing_value": ​-10000

},
{

    "name": "sun_zenith",
    "description": "250m 16 days sun zenith angle",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​-9000,
        "max": ​9000
    },
    "scale_factor": ​0.01,
    "missing_value": ​-10000

},
{

    "name": "azimuth",
    "description": "250m 16 days relative azimuth angle', , , 0.1, -4000",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​-3600,
        "max": ​3600
    },
    "scale_factor": ​0.10000000000000001,
    "missing_value": ​-4000

},
{

    "name": "day",
    "description": "250m 16 days composite day of the year",
    "datatype": "16-bit signed integer",
    "valid_range": 

    {
        "min": ​1,
        "max": ​366
    },
    "scale_factor": ​1,
    "missing_value": ​-1

},
{

    "name": "reliability",
    "description": "250m 16 days pixel reliability summary QA",
    "datatype": "8-bit signed integer",
    "valid_range": 

        {
            "min": ​0,
            "max": ​3
        },
        "scale_factor": ​1,
        "missing_value": ​-1
    }

],
"geo_extent": 
{

    "spatial": 

{

    "xmin": ​-20015109.354005989,
    "xmax": ​20015109.354005989,
    "ymin": ​-10007554.676994,
    "ymax": ​10007554.676994

},
"temporal": 

        {
            "start": "2000-02-18",
            "end": "2014-08-13",
            "resolution": ​16,
            "unit": "day"
        }
    }

}
```
```
http://www.dpi.inpe.br/wtss/time_series?coverage=mod09q1&attributes=red,nir&longitude=-54.0&latitude=-12.0&start=2001-01-01&end=2001-01-16
```
