/*
 * @Author: dega
 * @Date: 2022-02-14 17:38:10
 * @LastEditors: dega
 * @LastEditTime: 2022-02-14 17:48:58
 * @Description: 
 * @FilePath: \modern_cpp\coroutine.cpp
 */


#include <string>
#include <memory>
#include <iostream>
#include <coroutine>

// ur own data structure need default constructor and = operator
template<typename _type>
struct coroutine_data {
	coroutine_data() : __data{} {};
	virtual ~coroutine_data() {}
	auto get() const {
		return __data;
	}
	auto set(auto data) {
		__data = _type{ data };
	}
private:
	_type __data;
};

template<typename _type>
struct coroutine_promise {
	coroutine_promise() {
		__data = std::make_shared<coroutine_data<_type>>();
	}
	virtual ~coroutine_promise() {}
	//auto get_return_object() {
	//	std::cout << "get_return_object" << std::endl;
	//	return std::coroutine_handle<coroutine_promise>::from_promise(*this);
	//}
	auto initial_suspend() {
		std::cout << "initial_suspend" << std::endl;
		return std::suspend_always{};
	}
	auto final_suspend() noexcept(true) {
		std::cout << "final_suspend" << std::endl;
		return std::suspend_always{};
	}
	void return_void() {
		std::cout << "co_return return void" << std::endl;
	}
	//auto return_value(auto data) {
	//	std::cout << "return_value : " << data << std::endl;
	//	return __data->message();
	//}
	void unhandled_exception() {
		std::cout << "unhandled_exception" << std::endl;
		std::terminate();
	}
	auto yield_value(auto data) {
		std::cout << "yield_value : " << data << std::endl;
		__data->set(data);
		return std::suspend_always{};
	}
	std::shared_ptr<coroutine_data<_type>> __data;
};

template<typename _type>
struct coroutine_feature {
	coroutine_feature() = default;
	virtual ~coroutine_feature() {}
	auto data() {
		return __coroutine_handle.promise().__data->get();
	}
	auto create(auto handle) {
		__coroutine_handle = std::coroutine_handle<coroutine_promise<_type>>::from_address(handle.address());
	}
	auto destory() {
		__coroutine_handle.destroy();
	}
    auto resume() {
        if (!__coroutine_handle.done()) {
            __coroutine_handle.resume();
        }
        return !__coroutine_handle.done();
    }
	auto await_ready() const {
		return false;
	}
	auto await_suspend(std::coroutine_handle<> handle) {
		resume();
		return false;
	}
	auto await_resume() {
		return data();
	}
private:
	std::coroutine_handle<coroutine_promise<_type>> __coroutine_handle;
};

template<typename _type>
class derived_feature : public coroutine_feature<_type> {
public:
	struct promise_type : public coroutine_promise<_type> {
		virtual ~promise_type() = default;
		std::coroutine_handle<promise_type> get_return_object() {
			std::cout << "get_return_object" << std::endl;
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
	};
	derived_feature(derived_feature&&) = delete;
	derived_feature(const derived_feature&) = delete;
	derived_feature(std::coroutine_handle<promise_type> handle) {
        std::cout << "constructor " << std::endl;
		coroutine_feature<_type>::create(handle);
    }
    virtual ~derived_feature() {
        std::cout << "destructor " << std::endl;
    }
public:
	int dummy_function() {
		return 0;
	}
private:
	//int dummy_data;
};

struct own_data {
	int __dummy;
	auto data() {
		return __dummy;
	}
	~own_data() = default;
	own_data() : __dummy{ 0 } {}
	own_data(auto dummy) : __dummy{ dummy } {}
};

derived_feature<int> feature_int() {
	std::cout << "calling coroutine feature_int ..." << std::endl;
	co_yield 2022;
}

derived_feature<own_data> feature_owndata() {
	std::cout << "calling coroutine feature_owndata ..." << std::endl;
	co_yield 2022;
}

derived_feature<std::string> happy_new_year() {
	std::cout << "calling coroutine happy_new_year ..." << std::endl;
	co_yield "happy new year 2022";
}

derived_feature<int> simple_coroutine() {
	auto int_feature = feature_int();
	auto int_data = co_await int_feature;
	std::cout << "feature_int data : " << int_data << std::endl;
	auto owndata_feature = feature_owndata();
	auto owndata_data = co_await owndata_feature;
	std::cout << "feature_owndata data : " << owndata_data.data() << std::endl;
	auto newyear_feature = happy_new_year();
	auto newyear_data = co_await newyear_feature;
	std::cout << "happy_new_year data : " << newyear_data << std::endl;
}

int main()
{
	auto sc = simple_coroutine();
	auto r = sc.resume();

	return 0;
}