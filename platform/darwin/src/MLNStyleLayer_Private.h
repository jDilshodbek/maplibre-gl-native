#import <Foundation/Foundation.h>

#import "MLNStyleLayer.h"
#import "MLNStyleValue_Private.h"

#include <mbgl/layermanager/layer_factory.hpp>
#include <mbgl/style/layer.hpp>

NS_ASSUME_NONNULL_BEGIN

// A struct to be stored in the `peer` member of mbgl::style::Layer, in order to implement
// object identity. We don't store a MLNStyleLayer pointer directly because that doesn't
// interoperate with ARC. The inner pointer is weak in order to avoid a reference cycle for
// "pending" MLNStyleLayers, which have a strong owning pointer to the mbgl::style::Layer.
struct LayerWrapper {
  __weak MLNStyleLayer *layer;
};

/**
 Assert that the style layer is valid.

 This macro should be used at the beginning of any public-facing instance method
 of ``MLNStyleLayer`` and its subclasses. For private methods, an assertion is more appropriate.
 */
#define MLNAssertStyleLayerIsValid()                                                           \
  do {                                                                                         \
    if (!self.rawLayer) {                                                                      \
      [NSException raise:MLNInvalidStyleLayerException                                         \
                  format:@"Either this layer got invalidated after the style change or "       \
                         @"-[MLNStyle removeLayer:] has been called with this instance but "   \
                         @"another style layer instance was added with the same identifer. "   \
                         @"It is an error to send any message to this layer since  it cannot " \
                         @"be recovered after removal due to the identifier collision. "       \
                         @"Use unique identifiers for all layer instances including layers "   \
                         @"of different types."];                                              \
    }                                                                                          \
  } while (NO);

@class MLNStyle;

@interface MLNStyleLayer (Private)

/**
 Initializes and returns a layer with a raw pointer to the backing store,
 associated with a style.
 */
- (instancetype)initWithRawLayer:(mbgl::style::Layer *)rawLayer;

/**
 Initializes and returns a layer with an owning pointer to the backing store,
 unassociated from a style.
 */
- (instancetype)initWithPendingLayer:(std::unique_ptr<mbgl::style::Layer>)pendingLayer;

@property (nonatomic, readwrite, copy) NSString *identifier;

/**
 A raw pointer to the mbgl object, which is always initialized, either to the
 value returned by `mbgl::Map getLayer`, or for independently created objects,
 to the pointer value held in `pendingLayer`. In the latter case, this raw
 pointer value stays even after ownership of the object is transferred via
 `mbgl::Map addLayer`.
 */
@property (nonatomic, readonly) mbgl::style::Layer *rawLayer;

/**
 Adds the mbgl style layer that this object represents to the mbgl map below the
 specified `otherLayer`.

 Once a mbgl style layer is added, ownership of the object is transferred to the
 `mbgl::Map` and this object no longer has an active unique_ptr reference to the
 `mbgl::style::Layer`.
 */
- (void)addToStyle:(MLNStyle *)style belowLayer:(nullable MLNStyleLayer *)otherLayer;

/**
 Removes the mbgl style layer that this object represents from the mbgl map.

 When a mbgl style layer is removed, ownership of the object is transferred back
 to the ``MLNStyleLayer`` instance and the unique_ptr reference is valid again. It
 is safe to add the layer back to the style after it is removed.
 */
- (void)removeFromStyle:(MLNStyle *)style;

@end

namespace mbgl {

class LayerPeerFactory {
 public:
  virtual ~LayerPeerFactory() = default;
  /**
   Get the corresponding core layer factory.
   */
  virtual LayerFactory *getCoreLayerFactory() = 0;
  /**
   Creates an MLNStyleLayer instance with a raw pointer to the backing store.
   */
  virtual MLNStyleLayer *createPeer(style::Layer *) = 0;
};

}  // namespace mbgl

NS_ASSUME_NONNULL_END
