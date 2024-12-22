# Object ID Mapping in WorldScene::renderMap()

This document maps ranges of `objectId` values to their corresponding object categories and provides descriptions.

## Object ID Ranges and Corresponding Objects

| Object ID Range | Object Category | Size |
|-----------------|-----------------|------|
| 0               | **No Object**   | 1x1  |
| 1               | **Void**        | 1x1  |
| 1-19            | **Trees**       | 2x2  |
| 20-29           | **Fences**      | 1x1  |
| 30-39           | **Flowers**     | 1x1  |

0 -> No Object, No Collision  
1 -> Void (No Object, Collision)
