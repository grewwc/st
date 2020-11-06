/* get underlying type of a pointer */
template <typename T>
struct underlying_type
{
    using type = T;
};

template <typename T>
struct underlying_type<T *>
{
    using type = T;
};

template <typename T>
struct underlying_type<T **>
{
    using type = T;
};

template <typename T>
struct underlying_type<T ***>
{
    using type = T;
};

template <typename T>
struct underlying_type<T ****>
{
    using type = T;
};