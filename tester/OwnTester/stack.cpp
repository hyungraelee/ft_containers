/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunlee <hyunlee@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:36:47 by hyunlee           #+#    #+#             */
/*   Updated: 2021/11/24 20:36:48 by hyunlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stack.hpp"

#include <iostream>
#include <stack>
#include <string>

#ifndef TESTED_NAMESPACE
#define TESTED_NAMESPACE ft
#endif

#define TESTED_TYPE int
#define T_SIZE_TYPE typename TESTED_NAMESPACE::stack< T >::size_type

template < typename T >
void printContainers(TESTED_NAMESPACE::stack< T > st,
                     bool print_content = true) {
  const T_SIZE_TYPE size = st.size();

  std::cout << "size: " << size << std::endl;
  if (print_content) {
    std::cout << "Content was:" << std::endl;
    while (st.size() != 0) {
      std::cout << "- " << st.top() << std::endl;
      st.pop();
    }
  }
  std::cout << "###############################################" << std::endl;
}

int main() {
  std::cout << "////////// TEST STACK //////////" << std::endl;

  std::cout << "  == push / copy test ==" << std::endl;
  TESTED_NAMESPACE::stack< TESTED_TYPE > st;
  for (int i = 0; i < 5; ++i) {
    st.push(i * 3 + 1);
  }
  std::cout << "original stack\n" << std::endl;
  printContainers(st);
  TESTED_NAMESPACE::stack< TESTED_TYPE > st_copy(st);
  std::cout << "copied stack\n" << std::endl;
  printContainers(st_copy);

  std::cout << "  == pop / top / empty test ==" << std::endl;
  std::cout << "top: " << st_copy.top() << std::endl;
  std::cout << "pop" << std::endl;
  st_copy.pop();
  std::cout << "top: " << st_copy.top() << std::endl;
  std::cout << "pop" << std::endl;
  st_copy.pop();
  std::cout << "top: " << st_copy.top() << std::endl;
  std::cout << "pop" << std::endl;
  st_copy.pop();
  std::cout << "top: " << st_copy.top() << std::endl;
  std::cout << "pop" << std::endl;
  st_copy.pop();
  std::cout << "top: " << st_copy.top() << std::endl;
  std::cout << "pop" << std::endl;
  st_copy.pop();
  std::cout << "Is empty: " << (st_copy.empty() ? "OK" : "KO") << std::endl;

  std::cout << "  == relational operators test ==" << std::endl;
  TESTED_NAMESPACE::stack< TESTED_TYPE > lhs(st);
  TESTED_NAMESPACE::stack< TESTED_TYPE > rhs(st);

  std::cout << "lhs\n" << std::endl;
  printContainers(lhs);
  std::cout << "rhs\n" << std::endl;
  printContainers(rhs);

  std::cout << "operator ==" << std::endl;
  std::cout << ((lhs == rhs) ? "OK" : "KO") << std::endl;

  std::cout << "operator !=" << std::endl;
  std::cout << ((lhs != rhs) ? "OK" : "KO") << std::endl;

  std::cout << "operator <" << std::endl;
  std::cout << ((lhs < rhs) ? "OK" : "KO") << std::endl;

  std::cout << "operator <=" << std::endl;
  std::cout << ((lhs <= rhs) ? "OK" : "KO") << std::endl;

  std::cout << "operator >" << std::endl;
  std::cout << ((lhs > rhs) ? "OK" : "KO") << std::endl;

  std::cout << "operator >=" << std::endl;
  std::cout << ((lhs >= rhs) ? "OK" : "KO") << std::endl;

}
