//
// Created by 叶璨铭 on 2021/11/25.
//
template <typename Tp>
class Rect_ {
public:
    //! default constructor
    Rect_();
    Rect_(Tp _x, Tp _y, Tp _width, Tp _height);
    Rect_(const Rect_& r);
    Rect_(Rect_&& r) SUSTech_NOEXCEPT;  //移动构造应该写上noexcept，否则不会被调用。

    Rect_& operator=(const Rect_& r);
    Rect_& operator=(Rect_&& r) SUSTech_NOEXCEPT;

    //! area (width*height) of the rectangle
    Tp area() const;
    //! true if empty
    bool empty() const;

    //! conversion to another data type
    template <typename Tp2>
    explicit operator Rect_<Tp2>() const;

    template <typename Tp2_>
    friend std::ostream& operator<<(std::ostream& os, const Rect_<Tp2_>& rect);

    Tp x;       //!< x coordinate of the top-left corner
    Tp y;       //!< y coordinate of the top-left corner
    Tp width;   //!< width of the rectangle
    Tp height;  //!< height of the rectangle
};
using Recti = Rect_<int>;
using Rect = Recti;
//普通构造函数
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>::Rect_() : Rect_(0, 0, 0, 0) {}
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>::Rect_(Tp _x, Tp _y, Tp _width, Tp _height) : x(_x), y(_y), width(_width), height(_height) {}
//拷贝构造和移动构造函数
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>::Rect_(const Rect_& r) : Rect_(r.x, r.y, r.width, r.height) {}
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>::Rect_(Rect_&& r) SUSTech_NOEXCEPT : x(std::move(r.x)),
                                                                     y(std::move(r.y)),
                                                                     width(std::move(r.width), height(std::move(r.height))) {}
//拷贝赋值和移动赋值函数
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>& Rect_<Tp>::operator=(const Rect_& r) {
    x = r.x;
    y = r.y;
    width = r.width;
    height = r.height;
    return *this;
}
template <typename Tp>
SUSTech_ALWAYS_INLINE Rect_<Tp>& Rect_<Tp>::operator=(Rect_&& r) SUSTech_NOEXCEPT {
    x = std::move(r.x);
    y = std::move(r.y);
    width = std::move(r.width);
    height = std::move(r.height);
    return *this;
}
//类型转换
//要写两个template，是因为类本身只有一个模板参数，而不是两个。
template <typename Tp>
template <typename Tp2>
SUSTech_ALWAYS_INLINE Rect_<Tp>::operator Rect_<Tp2>() const {
    return Rect_<Tp2>(static_cast<Tp2>(x), static_cast<Tp2>(y), static_cast<Tp2>(width), static_cast<Tp2>(height));
}
template <typename Tp>
SUSTech_ALWAYS_INLINE Tp Rect_<Tp>::area() const {
    return width * height;  // int，uchar的话有可能溢出的。
}
template <typename Tp>
SUSTech_ALWAYS_INLINE bool Rect_<Tp>::empty() const {
    return width <= 0 || height <= 0;
}
template <typename Tp>
SUSTech_ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, const Rect_<Tp>& rect) {
    os << "Rect_<" << typeid(Tp).name() << ">{"
       << "x: " << rect.x << " y: " << rect.y << " width: " << rect.width << " height: " << rect.height << "}";
    return os;
}

/*    //    template <typename Tp>
//    class Scalar_ : public std::array<Tp, 4> {
//    public:
//        Scalar_(): std::array<Tp, 4>(){}
//        Scalar_(std::initializer_list<Tp> list): std::array<Tp, 4>{list[0], list[1], list[2], list[3]} {}
//    };*/