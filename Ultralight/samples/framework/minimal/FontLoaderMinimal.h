#include <Ultralight/platform/FontLoader.h>

namespace ultralight {

/**
 * Minimal FontLoader implementation that just returns an embedded font (Roboto).
 */
class FontLoaderMinimal : public FontLoader {
public:
  FontLoaderMinimal() {}
  virtual ~FontLoaderMinimal() {}
  virtual String16 fallback_font() const override;
  virtual Ref<Buffer> Load(const String16& family, int weight, bool italic, float size) override;
};

}  // namespace ultralight
