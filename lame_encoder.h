struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;

namespace wav2mp3 {

class lame_encoder
{
public:
  lame_encoder(int samples_per_second, int channels, int quality);
  ~lame_encoder() = default;

  operator lame_global_flags*() const
  {
    return flags_;
  }

private:
  class flags
  {
  public:
    flags();
    flags(flags const&) = delete;
    flags& operator=(flags const&) = delete;
    ~flags();

    operator lame_global_flags*() const
    {
      return global_flags_;
    }

  private:
    lame_global_flags* global_flags_;
  };
  flags flags_;
};
}
